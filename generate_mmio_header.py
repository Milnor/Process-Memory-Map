#!/usr/bin/env python3

import argparse
import subprocess
from dataclasses import dataclass

@dataclass
class Peripheral:
    """Class for PCI devices detected by lspci."""
    name: str
    address: str    # XX:yy.z Bus:Device.Function
    mem_start: int
    mem_end: int
    mem_length: int

    def __str__(self):
        red = "\033[31m"
        reset = "\033[0m"
        fields = f"{self.address} {self.name} {self.mem_start:#X} - {self.mem_end:#x} {self.mem_length}"
        if self.mem_length > 0:
            return fields
        else:
            return red + fields + reset

def parse_address_range(range: str):
    """Return the first start address, end address, and length."""
    first_line = range.split("\n")[0]
    start, end, flags = first_line.split()
    #print(f"{start} - {end} - {flags}")
    start = int(start, 16)
    end = int(end, 16)
    return start, end, end - start

def select_pci_device(header: str, lspci: str) -> str:
    all_peripherals = []
    devices = lspci.split("\n")
    #for index, device in enumerate(devices):
    #    print(f"{index}: {device}")
    for device in devices:
        if ":" in device: # omit junk entry at end of list
            address, long_name = device.split(" ", 1)
            short_name = long_name.split(":", 1)[0]
            #print(f"{address} - {short_name} - {long_name}")

        file_name = "/sys/bus/pci/devices/0000:" + address + "/resource"
        #args = ["lspci", file_name]
        mem_range = subprocess.run(["cat", file_name], capture_output=True)
        if mem_range.returncode != 0:
            print(f"[!] Failed to read {file_name}")
        else:
            #print(f"passing in:\n{mem_range.stdout.decode('utf-8')}")
            start, end, length = parse_address_range(mem_range.stdout.decode("utf-8"))
            peripheral = Peripheral(short_name, address, start, end, length)
            all_peripherals.append(peripheral)

    for index, device in enumerate(all_peripherals):
        print(f"{index: >2}: {device}")

    return header

def main(args):

    print(dir(args))

    default_header = '''
    #ifndef MMIO_H
    #define MMIO_H

    #define MMIO_BASE   0x0000
    #define MMIO_SIZE   0x1000
    #define DEVICE_NAME "SKIP"

    #endif // MMIO_H
    '''

    pci_devices = subprocess.run("lspci", capture_output=True)
    if pci_devices.returncode != 0:
        print(f"[!] lspci failed with {pci_devices.returncode}: {pci_devices.stderr}")
        print(f"[!] defaulting to dummy mmio.h")
    else:
        if args.non_interactive:
            # use hardcoded defaults that work on my box
            pass
        else:
            # interactive mode
            default_header = select_pci_device(default_header, pci_devices.stdout.decode("utf-8"))

    with open('mmio.h', 'w') as header:
        header.write(default_header)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--non-interactive", help="run as non-interactive script")
    args = parser.parse_args()
    main(args)

