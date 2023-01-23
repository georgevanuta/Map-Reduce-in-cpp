build:
	@g++ tema1.cpp helpers.cpp mappers.cpp reducers.cpp -o tema1 -Wall -lpthread -Werror

clean:
	@rm tema1 bonus1
