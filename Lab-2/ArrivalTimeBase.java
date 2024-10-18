public class ArrivalTimeBase {
    private static int arrivalIdCounter = 1;
    protected int arrivalId;

    public ArrivalTimeBase() {
        this.arrivalId = arrivalIdCounter++;
    }

    public int getArrivalId() {
        return arrivalId;
    }

    @Override
    public String toString() {
        return "T-" + arrivalId;
    }
}
