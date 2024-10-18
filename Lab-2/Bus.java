import java.util.*;
import java.util.concurrent.Semaphore;

public class Bus extends ArrivalTimeBase {
    private static int busIdCounter = 0;
    private int busId;
    private List<Passenger> passengers;
    private Semaphore semaphore;
    private Semaphore lock;
    private Semaphore finalLock;
    private Semaphore leaveLock;
    private boolean departing = false;
    private int capacity;

    public Bus(int capacity) {
        super();
        this.busId = busIdCounter++;
        this.capacity = capacity;
        this.passengers = new ArrayList<>();
        this.semaphore = new Semaphore(capacity);
        this.lock = new Semaphore(1);
        this.finalLock = new Semaphore(0);
        this.leaveLock = new Semaphore(0);
    }

    public List<Passenger> getPassengers() {
        return passengers;
    }

    public Semaphore getSemaphore() {
        return semaphore;
    }

    public Semaphore getLock() {
        return lock;
    }

    public Semaphore getFinalLock() {
        return finalLock;
    }

    public Semaphore getLeaveLock() {
        return leaveLock;
    }

    public boolean isDeparting() {
        return departing;
    }

    public void setDeparting(boolean departing) {
        this.departing = departing;
    }

    public int getCapacity() {
        return this.capacity;
    }

    @Override
    public String toString() {
        return "Bus " + busId + " - " + super.toString();
    }
}
