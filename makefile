.PHONY : generate
generate : sender  client

sender: sender.c
client: client.c 
listen: listen.c
server: server.c
