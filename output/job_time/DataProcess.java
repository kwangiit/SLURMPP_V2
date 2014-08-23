import java.io.*;

public class DataProcess {
	public static void main(String[] args) throws IOException {
		File dir = new File(".");
		String[] foundFiles = dir.list(new FilenameFilter() {
			public boolean accept(File dir, String name) {
				return name.startsWith("srun_job_time_");
				}
			});
		
		System.out.println("The number of jobs is:" + foundFiles.length);
		BufferedReader br = null;
		long minStartTime = Long.MAX_VALUE;
		long maxEndTime = Long.MIN_VALUE;
		
		for (String filename : foundFiles) {
			br = new BufferedReader(new FileReader(filename));
			String[] strLine = br.readLine().split(" ");
			long startTime = Long.parseLong(strLine[1]);
			long endTime = Long.parseLong(strLine[8]);
			//System.out.println("The startTime is:" + startTime + ", and the endTime is:" + endTime);
			if (startTime < minStartTime) {
				minStartTime = startTime;
			}
			if (endTime > maxEndTime) {
				maxEndTime = endTime;
			}
			br = null;
		}
		
		double overallTime = (double)((maxEndTime - minStartTime)) / 1E6;
		double throughput = foundFiles.length / overallTime;
		
		System.out.println("The throughput is:" + throughput);
	}

}
