# GoQuant Project

## Required Libraries

To build and run this project, you need the following libraries:

1. **cURL**: A library for transferring data with URLs.
2. **nlohmann/json**: A JSON library for modern C++.
3. **WebSocket++**: A C++ library for WebSocket client and server applications.

### Installation Instructions

#### cURL

cURL is used for making HTTP requests. You can install it using the following commands:

- On Ubuntu/Debian:
  ```sh
  sudo apt-get update
  sudo apt-get install libcurl4-openssl-dev
```
- On macOS:
  ```sh
  brew install curl
  ```

#### nlohmann/json

nlohmann/json is a header-only library, so you just need to include the header file in your project. You can download it from the GitHub repository or install it using a package manager:

- On Ubuntu/Debian:
  ```sh
  sudo apt-get update
  sudo apt-get install nlohmann-json-dev
```
- On macOS:
  ```sh
  brew install nlohmann-json
  ```

#### Boost

WebSocket++ is used for WebSocket communication. You can install it using the following commandsBoost is used for various functionalities including WebSocket communication. You can install it using the following commands:

- On Ubuntu/Debian:
  ```sh
  sudo apt-get update
  sudo apt-get install libboost-all-dev
```
- On macOS:
  ```sh
  brew install boost
```
## Building the Project

To build the project, navigate to the project directory and run the following command:

```sh
  make
```
This will compile the source files and generate the executable.


## Running the Project

Before running the project, make sure to set the CLIENT_ID and CLIENT_SECRET environment variables:

```sh
  export CLIENT_ID=your_client_id
  export CLIENT_SECRET=your_client_secret
```

Then, run the executable:

```sh
  ./GoQuant
```
### Usage
Usage
When you run the project, you will see a menu with the following options:

Place Order
Get Open Orders
Cancel Order
Modify Order
Get Order Book
View Current Positions
Subscribe to symbols

Enter the number corresponding to the action you want to perform and follow the prompts.

