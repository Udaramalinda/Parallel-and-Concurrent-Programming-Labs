import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.Comparator;

public class Simulation {
    public static double exponentialInterval(double mean) {
        return -mean * Math.log(1 - Math.random());
    }

    public static void main(String[] args) {
        // Check if correct number of arguments are passed
        if (args.length != 4) {
            System.out.println("Usage: java Simulation <noOfPassengers> <busCapacity> <pMean> <bMean>");
            return;
        }

        // Parse command-line arguments
        int noOfPassengers = Integer.parseInt(args[0]);
        int busCapacity = Integer.parseInt(args[1]);
        double pMean = Double.parseDouble(args[2]);
        double bMean = Double.parseDouble(args[3]);

        BusStop busStop = new BusStop();
        ExecutorService executor = Executors.newCachedThreadPool();

        int noOfBuses = 0;
        double nextPassengerTime = exponentialInterval(pMean);
        double nextBusTime = exponentialInterval(bMean);
        double currentTime = 0.0;
        int timeStepMs = 100;

        List<Bus> allBuses = new ArrayList<>();

        System.out.println("Simulation started\n\n");

        while (noOfPassengers > 0 || !busStop.getPassengers().isEmpty()) {
            try {
                TimeUnit.MILLISECONDS.sleep(timeStepMs);
                currentTime += timeStepMs / 1000.0;
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            while (currentTime >= nextPassengerTime && noOfPassengers > 0) {
                Passenger passenger = new Passenger();
                noOfPassengers--;
                busStop.addPassenger(passenger);
                System.out.println("Passenger " + passenger + " arrived at bus stop");
                executor.submit(() -> busStop.busBoard(passenger));
                nextPassengerTime += exponentialInterval(pMean);
            }

            while (currentTime >= nextBusTime && !busStop.getPassengers().isEmpty()) {
                Bus bus = new Bus(busCapacity);
                noOfBuses++;
                allBuses.add(bus);
                busStop.addBus(bus);
                System.out.println("Bus " + bus + " arrived at bus stop");
                executor.submit(() -> busStop.busFill(bus));
                nextBusTime += exponentialInterval(bMean);
            }
        }

        executor.shutdown();
        try {
            executor.awaitTermination(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        // Summary Section
        System.out.println("\nSummary\n");
        System.out.println("Total passengers: " + noOfPassengers + ", Total buses: " + noOfBuses);
        System.out.println("Total Simulated time: " + currentTime + "\n");

        for (Bus bus : allBuses) {
            if (bus.getPassengers().isEmpty()) {
                System.out.println("Bus " + bus + " had no passengers");
                continue;
            }

            // Finding the earliest and latest passengers
            Passenger earliestPassenger = bus.getPassengers().stream()
                .min(Comparator.comparingInt(Passenger::getArrivalId)).orElse(null);
            Passenger latestPassenger = bus.getPassengers().stream()
                .max(Comparator.comparingInt(Passenger::getArrivalId)).orElse(null);

            System.out.println("Bus " + bus + " had " + bus.getPassengers().size() + " passengers");
            System.out.println("Earliest arrival passenger " + earliestPassenger +
                ", Latest arrival passenger " + latestPassenger);
        }

        System.out.println("\nSimulation ended");
    }
}
