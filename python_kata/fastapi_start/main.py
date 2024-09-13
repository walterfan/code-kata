from fastapi import FastAPI, Depends, HTTPException, WebSocket, WebSocketDisconnect
from fastapi.security import OAuth2PasswordBearer, OAuth2PasswordRequestForm
from datetime import datetime, timedelta
from typing import Optional
from jose import JWTError, jwt
from passlib.context import CryptContext
import httpx  # 用于HTTP请求
from dotenv import load_dotenv
import os

load_dotenv()

# Secret key for JWT token (keep it secret in a real application)
SECRET_KEY = "secret-key-for-jwt"
ALGORITHM = "HS256"
ACCESS_TOKEN_EXPIRE_MINUTES = 30

# Password hashing context
pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

# Dummy database for users
fake_users_db = {
    "user1": {
        "username": "user1",
        "full_name": "User One",
        "email": "user1@example.com",
        "hashed_password": pwd_context.hash("password"),
        "disabled": False,
    },
}

oauth2_scheme = OAuth2PasswordBearer(tokenUrl="token")

app = FastAPI()

# 天气查询函数（使用第三方API）
async def get_weather(city: str) -> str:
    api_key = os.getenv("WEATHER_API_KEY")
    url = f"http://api.weatherapi.com/v1/current.json?key={api_key}&q={city}&aqi=no"
    async with httpx.AsyncClient() as client:
        response = await client.get(url)
        data = response.json()
        if response.status_code == 200:
            return f"City: {data['location']['name']}, Temperature: {data['current']['temp_c']}°C"
        else:
            return "Weather data not available."

# HTTP 路由：查询时间
@app.get("/time")
async def get_time():
    current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return {"time": current_time}

# HTTP 路由：查询天气
@app.get("/weather")
async def weather(city: str):
    weather_info = await get_weather(city)
    return {"weather": weather_info}

# WebSocket 连接管理
class ConnectionManager:
    def __init__(self):
        self.active_connections: list[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def send_personal_message(self, message: str, websocket: WebSocket):
        await websocket.send_text(message)

    async def broadcast(self, message: str):
        for connection in self.active_connections:
            await connection.send_text(message)

manager = ConnectionManager()

# WebSocket 路由：实时查询时间或天气
@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)
    try:
        while True:
            data = await websocket.receive_text()
            if data.lower() == "time":
                current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                await manager.send_personal_message(f"Current Time: {current_time}", websocket)
            elif data.lower().startswith("weather"):
                city = data.split(" ")[1] if len(data.split(" ")) > 1 else "London"
                weather_info = await get_weather(city)
                await manager.send_personal_message(f"Weather Info: {weather_info}", websocket)
            else:
                await manager.send_personal_message("Invalid command. Use 'time' or 'weather <city>'.", websocket)
    except WebSocketDisconnect:
        manager.disconnect(websocket)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
