Notes:
- Initially developed on OSX. Tested on CentOS 8.2 / 8.3.1. 
- Using boost 1.72 BOOST_ROOT=/opt/boost
- Using C++17 ( just because I never get to use structured bindings )
- Using boost::asio ( just using this project to learn something new )


Build: make all

Running client: 
client <symbol> [<ip>] [<port>]
e.g.: 
	client AMD@09:10
	client INTC@10:00 192.168.10.1 1333
	
Default port: 3333
Default ip: 127.0.0.1

Running server:
server

Env variables:
MD_SERVER_MODE=1 or 2
MD_SERVER_PORT=<port> 

Data file:
market_data.csv

Default mode: 1
Default port: 3333


