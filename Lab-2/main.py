import random
import time
from threading import Semaphore, Thread
import math


def exponential_intervals(mean):
    """Generate time intervals based on exponential distribution with the given mean."""
    return -mean * math.log(1 - random.random())


class ArrivalTimeBase:
    arrival_id = 1

    def __init__(self):
        self.arrival_id = ArrivalTimeBase.arrival_id
        ArrivalTimeBase.arrival_id += 1

    def __str__(self) -> str:
        return f"T-{self.arrival_id}"


class Bus(ArrivalTimeBase):
    _id = 0

    def __init__(self, capacity=50):
        self._id = self.__class__._id
        self.__class__._id += 1

        self.passengers = []
        self.semaphore = Semaphore(capacity)
        self.lock = Semaphore(1)
        self.final_lock = Semaphore(0)
        self.leave_lock = Semaphore(0)
        self.departing = False

        self.capacity = capacity

        super().__init__()

    def __eq__(self, __value: object) -> bool:
        return self._id == __value._id and isinstance(__value, Bus)

    def __str__(self) -> str:
        return f"Bus {self._id} - {super().__str__()}"

    def __repr__(self) -> str:
        return self.__str__()


class Passenger(ArrivalTimeBase):
    _id = 0

    def __init__(self):
        self._id = self.__class__._id
        self.__class__._id += 1
        self.bus = None

        super().__init__()

    def __str__(self) -> str:
        return f"Passenger {self._id} - {super().__str__()}"

    def __repr__(self) -> str:
        return self.__str__()


class BusStop:
    def __init__(self):
        self.buses = {}
        self.passengers = []
        self.lock = Semaphore(1)

    def get_bus(self, passenger: Passenger) -> Bus:
        for key, value in self.buses.items():
            if key > passenger.arrival_id:
                return value
        return None


def bus_board(bus_stop: BusStop, passenger: Passenger):
    while True:
        bus_stop.lock.acquire()
        if bus_stop.get_bus(passenger) is None:
            bus_stop.lock.release()
        else:
            bus = bus_stop.get_bus(passenger)
            bus_stop.lock.release()
            bus.semaphore.acquire()
            if not bus.departing:
                # print(
                #     f"Passenger {passenger} waiting to board bus {bus}\n",
                #     end="",
                # )

                break

            else:
                bus.semaphore.release()

    bus.passengers.append(passenger)
    passenger.bus = bus
    bus_stop.passengers.remove(passenger)

    print(f"Passenger {passenger} boarded bus {bus}\n", end="")

    bus.lock.acquire()
    if not bus.departing:
        if (
            len(
                [
                    passenger
                    for passenger in bus_stop.passengers
                    if passenger.arrival_id <= bus.arrival_id
                    and passenger.bus is not bus
                ]
            )
            == 0
            or len(bus.passengers) == bus.capacity
        ):
            bus.leave_lock.release()
            bus.final_lock.acquire()
            bus.departing = True
            bus.final_lock.release()
    bus.lock.release()

    # print(f"Passenger {passenger} left bus stop in bus\n", end="")


def bus_fill(bus: Bus, bus_stop: BusStop):
    bus.final_lock.release()

    while True:
        acquired = bus.leave_lock.acquire(timeout=0.2)

        if (
            len(
                [
                    passenger
                    for passenger in bus_stop.passengers
                    if passenger.arrival_id <= bus.arrival_id
                    and ((passenger.bus is bus) or (passenger.bus is None))
                ]
            )
            == 0
        ):
            bus.departing = True
            print(
                f"Bus {bus} left bus stop at capacity {len(bus.passengers)}\n", end=""
            )
            return

        if acquired:
            break

    #  Bus is full
    bus_stop.lock.acquire()
    bus_stop.buses.pop(bus.arrival_id)
    bus_stop.lock.release()
    print(f"Bus {bus} left bus stop at capacity {len(bus.passengers)}\n", end="")
    bus.lock.acquire()
    bus.semaphore.release()
    bus.lock.release()
    # print(f"{bus_stop.passengers}\n", end="")


if __name__ == "__main__":
    print("Starting the simulation\n\n", end="")

    bus_stop = BusStop()
    no_of_passengers = 101

    # Mean arrival times, used small values for convenience of quick simulation
    time_step = 0.1
    p_mean = 0.2
    b_mean = 2.0
    # same for bus capacity
    bus_capacity = 10

    all_threads = []
    all_buses = []

    remaining_passengers = no_of_passengers

    next_passenger_time = exponential_intervals(p_mean)
    next_bus_time = exponential_intervals(b_mean)
    current_time = 0.0

    no_of_buses = 0

    while remaining_passengers > 0:
        time.sleep(time_step)  # Simulate passage of time in small increments
        current_time += time_step

        # Check if it's time to add a passenger
        while current_time >= next_passenger_time and remaining_passengers > 0:
            remaining_passengers -= 1
            passenger = Passenger()
            bus_stop.passengers.append(passenger)
            print(f"Passenger {passenger} arrived at bus stop\n", end="")

            thread = Thread(target=bus_board, args=(bus_stop, passenger))
            thread.start()
            all_threads.append(thread)

            next_passenger_time += exponential_intervals(p_mean)

        # Check if it's time to add a bus
        while current_time >= next_bus_time and len(bus_stop.passengers) > 0:
            bus = Bus(capacity=bus_capacity)
            no_of_buses += 1
            all_buses.append(bus)
            bus_stop.lock.acquire()
            bus_stop.buses[bus.arrival_id] = bus
            bus_stop.lock.release()
            print(f"Bus {bus} arrived at bus stop\n", end="")

            thread = Thread(target=bus_fill, args=(bus, bus_stop))
            thread.start()
            all_threads.append(thread)

            next_bus_time += exponential_intervals(b_mean)
            
    # Keep sending buses till all passengers have boarded
    while len(bus_stop.passengers) > 0:
        time.sleep(time_step)  # Simulate passage of time in small increments
        current_time += time_step

        while current_time >= next_bus_time and len(bus_stop.passengers) > 0:
            bus = Bus(capacity=bus_capacity)
            no_of_buses += 1
            all_buses.append(bus)
            bus_stop.lock.acquire()
            bus_stop.buses[bus.arrival_id] = bus
            bus_stop.lock.release()
            print(f"Bus {bus} arrived at bus stop\n", end="")

            thread = Thread(target=bus_fill, args=(bus, bus_stop))
            thread.start()
            all_threads.append(thread)

            next_bus_time += exponential_intervals(b_mean)
            
    for thread in all_threads:
        thread.join()

    print("\nSummary\n")
    print(f"Total passengers: {no_of_passengers}, Total buses: {no_of_buses}\n")
    print(f"Total Simulated time: {current_time}\n")

    for bus in all_buses:
        if len(bus.passengers) == 0:
            print(f"Bus {bus} had no passengers\n", end="")
            continue

        earliest_pas = min(bus.passengers, key=lambda p: p.arrival_id)
        latest_pas = max(bus.passengers, key=lambda p: p.arrival_id)
        print(f"Bus {bus} had {len(bus.passengers)} passengers\n", end="")
        print(
            f"Earliest arrival passenger {earliest_pas}, Latest arrival passenger {latest_pas}\n"
        )

    print("\nSimulation ended")
