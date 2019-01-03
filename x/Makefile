.PHONY: all install clean

APP=x
APPS=$(APP).o

%.o: %.c
	cc -g -Wall -c $<

$(APP): $(APPS)
	cc -g -Wall -o $@ $^

install:
	cp $(APP) /usr/bin && chmod +s /usr/bin/$(APP)

clean:
	rm -f $(APP) *.o
