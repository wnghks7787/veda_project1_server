# 출결 관리 시스템 - 서버

출결 관리 시스템의 서버입니다.

## 📂 프로젝트 구조

- `server.h`: 서버 헤더 파일
- `server.cpp`: 서버 구현 파일
- `*.json`: 사용자 정보 및 출결 정보 파일 (로컬에 저장됨)

## 🚀 실행 방법

```bash
# 서버 실행
git clone https://github.com/your-username/veda_project1_server.git
cd veda_project1_server
qmake
make
./veda_project1_server
```

### 관리자 로그인 정보 추가
해당 Json 정보를 반드시 실행 서버(.exe파일)가 있는 폴더에 `user.json`으로 추가해주세요.

```json
[
    {
        "attendance": {
            "absent": 0,
            "be_out": 0,
            "early_leave": 0,
            "late": 0,
            "present": 0
        },
        "id": "admin",
        "info": {
            "age": 2027,
            "birthday": "",
            "name": "admin",
            "password": "admin",
            "phone_num": ""
        }
    }
]
```

## 🛠️ 기술 스택

- Qt 6.11.0
- C++