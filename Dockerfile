FROM ubuntu:22.04

# Install system packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libenet-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working dir
WORKDIR /app

# Copy entire project
COPY . .

# Create build directory and build
RUN mkdir -p build && cd build && cmake .. && make -j$(nproc)

# Default: run the game client
ENTRYPOINT ["./build/AstroEngine/AstroEngine"]
CMD ["client"]
