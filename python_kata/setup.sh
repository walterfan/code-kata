mkdir -p ~/.pip
cat <<EOF > ~/.pip/pip.conf
[global]
trusted-host =  mirrors.aliyun.com
index-url = http://mirrors.aliyun.com/pypi/simple
EOF
pip install --upgrade pip

virtualenv -p python3 venv
./venv/bin/activate
pip install -r requirements.txt
