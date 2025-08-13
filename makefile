.PHONY : generate
generate : sender  client listen server

sender: sender.c
client: client.c 
listen: listen.c
server: server.c
