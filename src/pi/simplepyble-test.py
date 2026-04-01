import simplepyble
import time

BLE_ADDRESS = "74:4D:BD:65:D3:E9"
SERVICE_UUID ="4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
DEFAULT_PI_BLE_ADAPTER_IDX = 0

def on_scan_start():
    print("Scan started.")
def on_scan_stopped():
    print("Scan complete.")
def on_device_found(peripheral):
    if(peripheral.address() == BLE_ADDRESS):
        print(f"Found {peripheral.identifier()} [{peripheral.address()}]")
def on_notification(data):
    print(f"Notification: {data}")
def main():
    adapters = simplepyble.Adapter.get_adapters()
    if len(adapters) == 0:
        print("No adapters found")
    adapter = adapters[DEFAULT_PI_BLE_ADAPTER_IDX]
    print(f"Selected Pi adapter: {adapter.identifier()} [{adapter.address()}]")
    adapter.set_callback_on_scan_start(on_scan_start)
    adapter.set_callback_on_scan_stop(on_scan_stopped)
    adapter.set_callback_on_scan_found(on_device_found)
    adapter.scan_for(5000)
    
    peripherals = adapter.scan_get_results()
    selectedperi = 0

    for i, peripheral in enumerate(peripherals):
        if(peripheral.address() == BLE_ADDRESS):
            selectedperi = peripheral
            
    peripheral = selectedperi
    print(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    peripheral.connect()
    services = peripheral.services()
    service_characteristic_pair = []
    for service in services:
        for characteristic in service.characteristics():
            service_characteristic_pair.append((service.uuid(), characteristic.uuid()))
    service_uuid = SERVICE_UUID
    characteristic_uuid = CHARACTERISTIC_UUID
    contents = peripheral.read(service_uuid, characteristic_uuid)
    print(f"Contents: {contents}")
    content = "231"
    peripheral.write_request(service_uuid, characteristic_uuid, str.encode(content))
    peripheral.notify(service_uuid, characteristic_uuid, on_notification)
    time.sleep(5)

if __name__ == "__main__":
    main()
