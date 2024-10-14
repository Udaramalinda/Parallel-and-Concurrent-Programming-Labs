import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) {
        BusStop busStop = new BusStop();
        ExecutorService executor = Executors.newCachedThreadPool();

        // Simulate bus and rider arrivals
        for (int i = 0; i < 100; i++) {
            executor.execute(new Rider(busStop));

            // Simulate random inter-arrival times (riders: 30 sec, buses: 20 min)
            try {
                TimeUnit.SECONDS.sleep((long) (Math.random() * 10)); // Rider inter-arrival
                if (i % 10 == 0) { // Assume a bus arrives every 10 riders
                    executor.execute(new Bus(busStop));
                    TimeUnit.SECONDS.sleep((long) (Math.random() * 30)); // Bus inter-arrival
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        executor.shutdown();
    }
}
