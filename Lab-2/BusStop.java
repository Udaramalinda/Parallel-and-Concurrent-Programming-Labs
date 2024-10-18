import java.util.*;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;


public class BusStop {
    private Map<Integer, Bus> buses;
    private List<Passenger> passengers;
    private Semaphore lock;

    public BusStop() {
        this.buses = new HashMap<>();
        this.passengers = new ArrayList<>();
        this.lock = new Semaphore(1);
    }

    public Bus getBus(Passenger passenger) {
        try {
            lock.acquire();
            for (Bus bus : buses.values()) {
                if (bus.getArrivalId() >= passenger.getArrivalId()) {
                    return bus;
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            lock.release();
        }
        return null;
    }

    public void addPassenger(Passenger passenger) {
        passengers.add(passenger);
    }

    public void addBus(Bus bus) {
        try {
            lock.acquire();
            buses.put(bus.getArrivalId(), bus);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            lock.release();
        }
    }

    public List<Passenger> getPassengers() {
        return passengers;
    }

    public Map<Integer, Bus> getBuses() {
        return buses;
    }

    public Semaphore getLock() {
        return lock;
    }

    public void busBoard(Passenger passenger) {
        Bus bus;
        while (true) {
            bus = getBus(passenger);
            if (bus != null) {
                try {
                    bus.getSemaphore().acquire();
                    if (!bus.isDeparting()) {
                        break;
                    } else {
                        bus.getSemaphore().release();
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }

        synchronized (this) {
            bus.getPassengers().add(passenger);
            passenger.setBus(bus);
            passengers.remove(passenger);
        }

        System.out.println("Passenger " + passenger + " boarded bus " + bus);

        try {
            bus.getLock().acquire();
            if (!bus.isDeparting() && (passengers.isEmpty() || bus.getPassengers().size() == bus.getCapacity())) {
                bus.getLeaveLock().release();
                bus.getFinalLock().acquire();
                bus.setDeparting(true);
                bus.getFinalLock().release();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            bus.getLock().release();
        }
    }

    public void busFill(Bus bus) {
        bus.getFinalLock().release();
        while (true) {
            try {
                boolean acquired = bus.getLeaveLock().tryAcquire(200, TimeUnit.MILLISECONDS);

                List<Passenger> waitingPassengers = new ArrayList<>();
                for (Passenger passenger : passengers) {
                    if (passenger.getArrivalId() <= bus.getArrivalId() &&
                        (passenger.getBus() == null || passenger.getBus() == bus)) {
                        waitingPassengers.add(passenger);
                    }
                }

                if (waitingPassengers.isEmpty()) {
                    bus.setDeparting(true);
                    System.out.println(bus + " left bus stop with " + bus.getPassengers().size() + " passengers.");
                    return;
                }

                if (acquired) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        try {
            lock.acquire();
            buses.remove(bus.getArrivalId());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            lock.release();
        }

        System.out.println(bus + " left bus stop with " + bus.getPassengers().size() + " passengers.");

        try {
            bus.getLock().acquire();
            bus.getSemaphore().release();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            bus.getLock().release();
        }
    }
}
