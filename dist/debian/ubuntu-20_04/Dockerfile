# Based on the CircleCI Ubuntu 20.04 image
FROM cimg/base:stable-20.04

# Update all packages and install build dependencies
RUN echo "Updating all Ubuntu packages..."; sudo apt update && sudo apt upgrade -y; sudo apt-get clean
RUN echo "Installing build dependencies..."; sudo apt install -y cmake
