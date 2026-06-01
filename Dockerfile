FROM gcc:14

WORKDIR /app

COPY . .

RUN g++ -std=c++17 backend/main.cpp -o backend/main -lws2_32

EXPOSE 8080

CMD ["backend/main"]