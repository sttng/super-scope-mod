import simplepyble
import time

DEFAULT_PI_ADAPTER_IDX = 0
def on_scan_start():
    print("Scan started.")
def on_scan_stopped():
    print("Scan complete.")
def on_device_found(peripheral):
    print(f"Found {peripheral.identifier()} [{peripheral.address()}]")
def on_notification(data):
    print(f"Notification: {data}")
def main():
    adapters = simplepyble.Adapter.get_adapters()
    if len(adapters) == 0:
        print("No adapters found")
    adapter = adapters[DEFAULT_PI_ADAPTER_IDX]
    print(f"Selected adapter: {adapter.identifier()} [{adapter.address()}]")
    adapter.set_callback_on_scan_start(on_scan_start)
    adapter.set_callback_on_scan_stop(on_scan_stopped)
    adapter.set_callback_on_scan_found(on_device_found)
    adapter.scan_for(5000)
    peripherals = adapter.scan_get_results()
    print("Please select a peripheral:")
    for i, peripheral in enumerate(peripherals):
        print(f"{i}: {peripheral.identifier()} [{peripheral.address()}]")
    choice = int(input("Enter choice: "))
    peripheral = peripherals[choice]
    print(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    peripheral.connect()
    services = peripheral.services()
    service_characteristic_pair = []
    for service in services:
        for characteristic in service.characteristics():
            service_characteristic_pair.append((service.uuid(), characteristic.uuid()))
    print("Please select a service/characteristic pair:")
    for i, (service_uuid, characteristic) in enumerate(service_characteristic_pair):
        print(f"{i}: {service_uuid} {characteristic}")
    choice = int(input("Enter choice: "))
    service_uuid, characteristic_uuid = service_characteristic_pair[choice]
    contents = peripheral.read(service_uuid, characteristic_uuid)
    print(f"Contents: {contents}")
    content = input("Enter content to write: ")
    peripheral.write_request(service_uuid, characteristic_uuid, str.encode(content))
    peripheral.notify(service_uuid, characteristic_uuid, on_notification)
    time.sleep(5)

if __name__ == "__main__":
    main()
