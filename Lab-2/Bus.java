import java.util.concurrent.Semaphore;

public class Bus implements Runnable {
    private final BusStop busStop;

    public Bus(BusStop busStop) {
        this.busStop = busStop;
    }

    @Override
    public void run() {
        try {
            busStop.arrive();
            busStop.depart();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
