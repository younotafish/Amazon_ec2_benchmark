gcc -o client.out client.c && chmod +x client.out
gcc -o server.out server.c && chmod +x server.out

./server.out

# ./client.out 127.0.0.1 8000
