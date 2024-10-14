import java.util.concurrent.Semaphore;

public class Rider implements Runnable {
    private final BusStop busStop;

    public Rider(BusStop busStop) {
        this.busStop = busStop;
    }

    @Override
    public void run() {
        try {
            busStop.waitForBus();
            busStop.boardBus();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
