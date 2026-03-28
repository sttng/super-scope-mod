
# only Python
```
pip3 install opencv-contrib-python
```

# If building yourself


You need at least 2GB ZRAM. 

# Install zram tools

```
sudo apt install zram-tools
```

# Disable the current swap

```
sudo swapoff /dev/zram0
```

# Reset the device

```
sudo zramctl --reset /dev/zram0
```

# Set the new size and re-initialize as swap

```
sudo zramctl --find --size 2G --algorithm zstd
sudo mkswap /dev/zram0
sudo swapon /dev/zram0
```


https://qengineering.eu/install-opencv-lite-on-raspberry-pi.html
