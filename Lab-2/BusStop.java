import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BusStop {
    private final int MAX_CAPACITY = 50;
    private final Semaphore riderSemaphore = new Semaphore(0);  // Controls riders boarding
    private final Semaphore busSemaphore = new Semaphore(0);    // Controls when the bus can depart
    private final Lock mutex = new ReentrantLock();
    private int waitingRiders = 0;

    // Rider waits for a bus to arrive
    public void waitForBus() throws InterruptedException {
        mutex.lock();
        waitingRiders++;
        mutex.unlock();
        riderSemaphore.acquire();  // Wait for bus to signal boarding
    }

    // Rider boards the bus
    public void boardBus() {
        mutex.lock();
        waitingRiders--;
        System.out.println("Rider boarded the bus. Riders left: " + waitingRiders);
        if (waitingRiders == 0 || riderSemaphore.availablePermits() == 0) {
            busSemaphore.release();  // Allow bus to depart when all riders have boarded
        }
        mutex.unlock();
    }

    // Bus arrives at the bus stop
    public void arrive() throws InterruptedException {
        mutex.lock();
        System.out.println("Bus arrived. Waiting riders: " + waitingRiders);
        int ridersToBoard = Math.min(waitingRiders, MAX_CAPACITY);
        riderSemaphore.release(ridersToBoard);  // Allow riders to board
        mutex.unlock();
        busSemaphore.acquire();  // Wait for riders to finish boarding
    }

    // Bus departs after boarding riders
    public void depart() {
        System.out.println("Bus is departing with passengers.");
    }
}
