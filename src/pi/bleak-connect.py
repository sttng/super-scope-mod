# Read the current state of a characteristic on a BLE device

import asyncio
from bleak import BleakClient

async def main():
    ble_address = "D0:CF:13:2F:C3:C5"
    characteristic_uuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

    async with BleakClient(ble_address) as client:
        data = await client.read_gatt_char(characteristic_uuid)
        value_str = "231"
        data = value_str.encode("utf-8")
        await client.write_gatt_char(characteristic_uuid, data)

asyncio.run(main())
