
sudo apt-get install python3-pip

python3 -m venv venv
source venv/bin/activate


pip3 install bleak



#Start Bluetooth Service:

sudo systemctl start bluetooth
#Unblock Bluetooth (if necessary):

sudo rfkill unblock bluetooth

Activate Power within bluetoothctl:
#bluetoothctl
Type power on
Type agent on
Type default-agent
Type exit




/usr/local/bin/bleak-lescan


# only Python
Option 1 - Main modules package: pip install opencv-python 

Option 2 - Full package (contains both main modules and contrib/extra modules): pip install opencv-contrib-python (check contrib/extra modules listing from OpenCV documentation)

Packages for server (headless) environments:

Option 3 - Headless main modules package: pip install opencv-python-headless

Option 4 - Headless full package (contains both main modules and contrib/extra modules): pip install opencv-contrib-python-headless




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
