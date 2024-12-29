
## Introduction

### Overview
8Bit Fighter is a multiplayer game that leverages SDL and SDL_net libraries to enable engaging networked gameplay. It features:
- Multiplayer support via TCP/UDP protocols.
- Retro 8-bit style graphics.
- Cross-platform compatibility.

### Repository Link
[GitHub Repository](https://github.com/anakin004/8Bit-Fighter)

## Getting Started

### Prerequisites
Before you begin, ensure you have the following installed:
- **SDL2**: The Simple DirectMedia Layer library.
- **SDL_net**: A network library for SDL.
- **C++ Compiler**: Compatible with the project's build system.

### Installation
1. **Clone the Repository:**
    ```bash
    git clone https://github.com/anakin004/8Bit-Fighter.git
    ```
2. **Navigate to the Project Directory:**
    ```bash
    cd 8Bit-Fighter
    ```
3. **Build the Project:**
   - If a `Makefile` is provided:
     ```bash
     make
     ```
   - For custom build instructions, refer to the `README.md` or project files.

## Configuration

### Server Setup
1. Identify the host's IP address (either private or public, depending on the visibility required).
2. Ensure all players can access the host's IP.
3. Run the server application.

### Client Setup
1. Launch the client application.
2. Input the host's IP address to establish a connection.
3. Adjust any settings as necessary for your setup.

## Gameplay

### Controls
Provide detailed information about the player controls:
- **Movement**: Arrow keys or WASD (customizable).
- **Actions**: Specify key bindings for attack, defend, etc.


## Development

### Project Structure
A high-level overview of the repository layout:
```
8Bit-Fighter/
├── dependencies/    # SDL2 dlls
├── fonts/           # font assets
├── headers/         # SDL headers and src headers
├── lib/             # static SDL2 libraries
├── src/             # Source code files
├── assets/          # Graphics and sound resources
└── README.md        # Project introduction and quick start guide
```

### Code Architecture
- **Main Components:**
  - `main.cpp`: Entry point of the application.
  - `network.cpp, server.cpp`: Handles client, server communication

### Contributing
1. Fork the repository on GitHub.
2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Commit your changes with descriptive messages:
   ```bash
   git commit -m "Add feature: your-feature-name"
   ```
4. Push the changes:
   ```bash
   git push origin feature/your-feature-name
   ```
5. Create a pull request on the original repository.

## Troubleshooting

### Common Issues
- **Connection Errors:** Ensure the server IP is correct and accessible, ensure correct ports are opended and set for all users, using WireShark is very useful in tracking packets
- **Latency:** Using an ethernet cable can reduce latency
- **Hosting:** Hosting can be done locally, but this is risky. Ensure additional restrictions: ex -> only accepting packets from select IP's 
- **Compilation Failures:** Verify SDL2 and SDL_net are installed and properly linked.





https://github.com/user-attachments/assets/3361465f-fc8d-43b3-ab6f-dd55dc6e8026

