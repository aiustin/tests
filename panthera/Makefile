
all: client server
	
.PHONY: clean
clean: client_clean server_clean
	
.PHONY: cleandep
cleandep: client_cleandep server_cleandep

.PHONY: cleanall
cleanall: client_cleanall server_cleanall


.PHONY: client
client:
	$(MAKE) -C client

.PHONY: server
server:
	$(MAKE) -C server

.PHONY: client_clean
client_clean:
	$(MAKE) clean -C client

.PHONY: server_clean
server_clean:
	$(MAKE) clean -C server

.PHONY: client_cleandep
client_cleandep:
	$(MAKE) cleandep  -C client

.PHONY: server_cleandep
server_cleandep:
	$(MAKE) cleandep -C server

.PHONY: client_cleanall
client_cleanall:
	$(MAKE) cleanall -C client

.PHONY: server_cleanall
server_cleanall:
	$(MAKE) cleanall -C server
