Here's a basic implementation of a web application using FastAPI that provides CRUD (Create, Retrieve, Update, Delete) operations for user accounts, including fields like `username`, `password`, and `role`. This example uses SQLAlchemy for database interactions and Pydantic models for request and response validation.

### 1. Install Required Packages

First, make sure you have the necessary packages installed:

```bash
pip install fastapi uvicorn sqlalchemy alembic pydantic bcrypt python-multipart
```

### 2. Setup the Database

We'll use SQLite for simplicity, but you can replace it with any other database supported by SQLAlchemy.

**models.py** (SQLAlchemy models):

```python
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

class User(Base):
    __tablename__ = "users"
    
    id = Column(Integer, primary_key=True, index=True)
    username = Column(String, unique=True, index=True)
    hashed_password = Column(String)
    role = Column(String)
```

### 3. Create Pydantic Schemas

**schemas.py**:

```python
from pydantic import BaseModel

class UserBase(BaseModel):
    username: str
    role: str

class UserCreate(UserBase):
    password: str

class UserUpdate(UserBase):
    password: str = None

class UserInDB(UserBase):
    id: int
    hashed_password: str

    class Config:
        orm_mode = True
```

### 4. Create CRUD Operations

**crud.py**:

```python
from sqlalchemy.orm import Session
from . import models, schemas
from .security import get_password_hash, verify_password

def create_user(db: Session, user: schemas.UserCreate):
    hashed_password = get_password_hash(user.password)
    db_user = models.User(username=user.username, hashed_password=hashed_password, role=user.role)
    db.add(db_user)
    db.commit()
    db.refresh(db_user)
    return db_user

def get_user(db: Session, user_id: int):
    return db.query(models.User).filter(models.User.id == user_id).first()

def get_user_by_username(db: Session, username: str):
    return db.query(models.User).filter(models.User.username == username).first()

def update_user(db: Session, user_id: int, user_update: schemas.UserUpdate):
    db_user = get_user(db, user_id)
    if user_update.password:
        db_user.hashed_password = get_password_hash(user_update.password)
    db_user.role = user_update.role
    db.commit()
    db.refresh(db_user)
    return db_user

def delete_user(db: Session, user_id: int):
    db_user = get_user(db, user_id)
    db.delete(db_user)
    db.commit()
    return db_user
```

### 5. Implement Password Hashing

**security.py**:

```python
from passlib.context import CryptContext

pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

def get_password_hash(password):
    return pwd_context.hash(password)

def verify_password(plain_password, hashed_password):
    return pwd_context.verify(plain_password, hashed_password)
```

### 6. Setup the FastAPI Application

**main.py**:

```python
from fastapi import FastAPI, Depends, HTTPException
from sqlalchemy.orm import Session
from . import models, schemas, crud
from .database import engine, SessionLocal

models.Base.metadata.create_all(bind=engine)

app = FastAPI()

# Dependency to get DB session
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@app.post("/users/", response_model=schemas.UserInDB)
def create_user(user: schemas.UserCreate, db: Session = Depends(get_db)):
    db_user = crud.get_user_by_username(db, username=user.username)
    if db_user:
        raise HTTPException(status_code=400, detail="Username already registered")
    return crud.create_user(db=db, user=user)

@app.get("/users/{user_id}", response_model=schemas.UserInDB)
def read_user(user_id: int, db: Session = Depends(get_db)):
    db_user = crud.get_user(db=db, user_id=user_id)
    if db_user is None:
        raise HTTPException(status_code=404, detail="User not found")
    return db_user

@app.put("/users/{user_id}", response_model=schemas.UserInDB)
def update_user(user_id: int, user: schemas.UserUpdate, db: Session = Depends(get_db)):
    db_user = crud.get_user(db=db, user_id=user_id)
    if db_user is None:
        raise HTTPException(status_code=404, detail="User not found")
    return crud.update_user(db=db, user_id=user_id, user_update=user)

@app.delete("/users/{user_id}", response_model=schemas.UserInDB)
def delete_user(user_id: int, db: Session = Depends(get_db)):
    db_user = crud.get_user(db=db, user_id=user_id)
    if db_user is None:
        raise HTTPException(status_code=404, detail="User not found")
    return crud.delete_user(db=db, user_id=user_id)
```

### 7. Setup the Database Connection

**database.py**:

```python
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

SQLALCHEMY_DATABASE_URL = "sqlite:///./test.db"

engine = create_engine(SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False})
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
```

### 8. Run the Application

Finally, run the application using Uvicorn:

```bash
uvicorn main:app --reload
```

You can now access the FastAPI documentation at `http://127.0.0.1:8000/docs` and test the CRUD operations for user accounts. 

This example covers the basic CRUD operations, but you can expand it by adding authentication, role-based access control, and more advanced features as needed.