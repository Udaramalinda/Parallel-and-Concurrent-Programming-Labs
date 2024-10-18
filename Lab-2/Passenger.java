public class Passenger extends ArrivalTimeBase {
    private static int passengerIdCounter = 0;
    private int passengerId;
    private Bus bus;

    public Passenger() {
        super();
        this.passengerId = passengerIdCounter++;
    }

    public Bus getBus() {
        return bus;
    }

    public void setBus(Bus bus) {
        this.bus = bus;
    }

    @Override
    public String toString() {
        return "Passenger " + passengerId + " - " + super.toString();
    }
}
