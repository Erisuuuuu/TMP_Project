FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN apt-get update && apt-get install -y \
    build-essential \
    qtbase5-dev \
    qttools5-dev-tools \
    libqt5websockets5-dev \
    libqt5sql5-sqlite \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
RUN git clone https://github.com/Erisuuuuu/TMP_Project . && \
    git submodule update --init --recursive

# Применяем патч для исправления SMTPClient
RUN sed -i 's/connect(\&socket, \&QSslSocket::sslErrors, \[this\]/connect(\&socket, static_cast<void(QSslSocket::*)(const QList<QSslError>\&)>(\&QSslSocket::sslErrors), \[this\]/g' /app/server/src/smtpclient.cpp

# Сборка сервера
WORKDIR /app/server/src
RUN qmake && make clean && make

# Сборка клиента
WORKDIR /app/client/src
RUN qmake && make clean && make

# Установка переменных для X11 (если нужен GUI)
ENV DISPLAY=host.docker.internal:0
ENV QT_X11_NO_MITSHM=1

EXPOSE 33337

CMD ["sh", "-c", "(cd /app/server/src && ./myserver &) && cd /app/client/src && ./client_window"]