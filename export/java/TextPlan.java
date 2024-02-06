package io.substrait;

public class SubstraitPlanLoader {
    enum PlanFileFormat {
      kBinary = 0,
      kJson = 1,
      kProtoText = 2,
      kText = 3,
    };

	// Load a Substrait plan (in any format) from disk.
	// Returns error message on failure, otherwise returns nullptr and
	// updates the provided buffer.
	public native String loadSubstraitPlan(String filename);

    // Write a Substrait plan to disk in the specified format.
	public native String saveSubstraitPlan(Plan plan, String filename, PlanFileFormat format);

	static {
		System.load("/path/to/libMYSTUFF.so");
	}

	public static void main(String [] args) {
		SubstraitPlanLoader loader = new SubstraitPlanLoader();

		String result = loader.loadSubstraitPlan("filename.plan");
		System.out.println("Result: " + result);
	}
}
