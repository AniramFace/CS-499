
public class Dog extends Pet{

	public static void main(String[] args) {}
		// TODO Auto-generated method stub
		/**
		 * 
		 * created attributes for dogs
		 *
		 */
			private int dogSpaceNumber;
			private double dogWeight;
			private String grooming;

		/**
		 * Set initial value
		 */
			public Dog(String petType, String petName, int petAge, int dogSpaces, int daysStay, int amountDue) {
				super(petType,petName,petAge,daysStay);
				setdogSpaceNumber(0);
				setdogWeight(0);
				setgrooming (grooming);
				
			}
			/**
			 * get method
			 */
			public int getdogSpaceNumber() {
				return dogSpaceNumber;
			}
			/**
			 * set method
			 */
			public void setdogSpaceNumber(int dogSpaceNumber) {
				this.dogSpaceNumber = dogSpaceNumber;
				/**
				 * get method
				 */	
			}
			public double getdogWeight1() {
				return dogWeight;
			}
			/**
			 * set method
			 */
			public void setdogWeight(double dogWeight) {
				this.dogWeight = dogWeight;
				/**
				 * get method
				 */	
			}
			public String getgrooming() {
				return grooming;
			}
			/**
			 * set method
			 */
			public void setgrooming(String grooming) {
				this.grooming = grooming;
				
			}
			
			{
}
				
		}

