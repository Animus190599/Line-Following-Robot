using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Text;
using System.Linq;
using System.IO;

namespace BigAss
{
    /// <summary>
    /// Read input stations from files and output the shortest distance to travel through all stations.
    /// Contains level 1, 2, 3, 4 implementation 
    /// </summary>
    class Program
    {
        static List<Station> stations = new List<Station>();
        static Plane plane;
        static TimeSpan curTime;
        static Tour tour;
        static Tour newTour;
        static Random rnd = new Random();
        public static void Main(string[] args)
        {

            var watch = System.Diagnostics.Stopwatch.StartNew();
            string rdPath = "D:/Animus_files/Documents/CAB201/" + args[0];
            string planePath = "D:/Animus_files/Documents/CAB201/" + args[1];
            try
            {
                curTime = TimeSpan.Parse(args[2]);
            }
            catch (FormatException)
            {
                Console.WriteLine("Unable to parse '{0}'", args[2]);
            }
            catch (OverflowException)
            {
                Console.WriteLine("'{0}' is outside the range of a TimeSpan.", args[2]);
            }
            // Call the method to read data from mail file and plane specs
            Program.ReadStationsFromFile(rdPath);
            Program.ReadPlaneSpecFromFile(planePath);
            // if more than 12 stations, run level 1 and 2 or level 1 and 4
            if (stations.Count > 12)
            {
                //Level 1--------------------------------------------------------
                tour = new Tour(SimpleHeuristic(stations), plane);
                //Level 2-------------------------------------------------------

                //newTour = ImprovedHeuristic(tour);
                //stations.Add(stations[0]);
                //tour = new Tour(stations, plane);
                newTour = SimulatedAnnealing(tour);


            }

            //Level 3 --------------------------------------------------------
            else
            {
                tour = new Tour(stations, plane);

                ExhaustiveSearch(tour);

                newTour.station.Add(stations[0]);
                newTour.station.Insert(0, stations[0]);
            }
            //Level 4 -------------------------------------------------------------------
            //SimulatedAnnealing(tour);
            //Stop watch;
            watch.Stop();
            if (args[3] == "-o" && args.Length == 5)
            {
                string wrPath = "D:/Animus_files/Documents/CAB201/" + args[4];
                Program.WriteMessageToFile(newTour, wrPath, watch.ElapsedMilliseconds);
            }

            Console.ReadLine();
        }

        /// <summary>
        /// Read stations information from the given file name
        /// </summary>
        /// <param name="rdPath">File name/Path of the file starting from the current folder</param>
        public static void ReadStationsFromFile(string rdPath)
        {

            try
            {
                using (StreamReader mySR = new StreamReader(new FileStream(rdPath, FileMode.Open, FileAccess.Read)))
                {
                    while (!mySR.EndOfStream)
                    {
                        //Split each line into different string, and store into the list of station class
                        string[] splitLine = mySR.ReadLine().Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                        Program.stations.Add(new Station(splitLine[0], float.Parse(splitLine[1]), float.Parse(splitLine[2])));
                    }
                }
            }
            //output error if can not find file
            catch (FileNotFoundException e)
            {
                Console.WriteLine(e.Message);
            }
        }

        /// <summary>
        /// Read plane specs from the given file name
        /// </summary>
        /// <param name="planePath">Given path of the plane-spec file</param>
        public static void ReadPlaneSpecFromFile(string planePath)
        {

            try
            {
                using (StreamReader mySR = new StreamReader(new FileStream(planePath, FileMode.Open, FileAccess.Read)))
                {
                    while (!mySR.EndOfStream)
                    {
                        string[] splitLine = mySR.ReadLine().Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                        //  Console.WriteLine("{0} {1} {2} {3} {4}", float.Parse(splitLine[0]), float.Parse(splitLine[1]), float.Parse(splitLine[2]), float.Parse(splitLine[3]), float.Parse(splitLine[4]));
                        plane = new Plane(float.Parse(splitLine[0]), float.Parse(splitLine[1]), float.Parse(splitLine[2]), float.Parse(splitLine[3]), float.Parse(splitLine[4]));
                    }
                }
            }
            catch (FileNotFoundException e)
            {
                Console.WriteLine(e.Message);
            }
        }

        /// <summary>
        /// Write the message to the given file name (if required)
        /// </summary>
        /// <param name="tour">Tour that has been reorganized to have shortest distance</param>
        /// <param name="wrPath">Path/ output file name</param>
        /// <param name="watch">elapse time</param>
        public static void WriteMessageToFile(Tour tour, string wrPath, long watch)
        {
            using (StreamWriter mySW = new StreamWriter(new FileStream(wrPath, FileMode.Create, FileAccess.ReadWrite)))
            {

                List<string> myList = new List<string>();
                TimeSpan totalTime = new TimeSpan(0, 0, 0);
                TimeSpan operationTime = plane.Convert(plane.range, "hh");
                TimeSpan curOperationTime = operationTime;
                TimeSpan fuelTime = plane.Convert(plane.fuel, "mm");
                float totalDist = 0;
                string time1, time2;
                char[] splitChar = { ':', '.', ' ' };
                string[] timePrint;
                for (int i = 0; i < tour.station.Count - 1; ++i)
                {

                    // Console.WriteLine(tour.PathTime(i));
                    totalTime += TimeSpan.Parse(tour.PathTime(i));
                    //Console.WriteLine(totalTime.ToString());
                    //Console.WriteLine(curOperationTime.ToString());

                    if (TimeSpan.Compare(totalTime, curOperationTime) > 0)
                    {
                        curTime += fuelTime;
                        //Console.WriteLine(curOperationTime.ToString());
                        curOperationTime = totalTime - TimeSpan.Parse(tour.PathTime(i)) + fuelTime + operationTime;
                        //Console.WriteLine(totalTime.ToString());
                        totalTime += fuelTime;
                        myList.Add(string.Format("***refuel {0} minutes ***", fuelTime.ToString().Split(':')[1]));
                    }
                    time1 = curTime.Hours.ToString("D2") + ":" + curTime.Minutes.ToString("D2");
                    curTime += TimeSpan.Parse(tour.PathTime(i));
                    time2 = curTime.Hours.ToString("D2") + ":" + curTime.Minutes.ToString("D2");

                    myList.Add(string.Format("{0}\t->\t{1}\t{2}\t{3}", tour.station[i].Name, tour.station[i + 1].Name, time1, time2));
                    totalDist += tour.station[i].Distance(tour.station[i + 1]);
                }

                mySW.WriteLine("Optimising tour length: Level 100...Elapsed time: {0} milliseconds", watch);

                timePrint = totalTime.ToString().Split(splitChar);
                for (int i = 0; i < timePrint.Length; i++)
                {
                    Console.WriteLine(totalTime.ToString());
                    Console.WriteLine(timePrint[i]);
                }
                if (timePrint.Length > 3)
                {
                    mySW.WriteLine("Tour time: {0} days {1} hours {2} minutes", timePrint[0], timePrint[1], timePrint[2]);
                }
                else
                {
                    mySW.WriteLine("Tour time: {0} hours {1} minutes", timePrint[0], timePrint[1]);
                }
                mySW.WriteLine("Tour length: {0}", totalDist);
                foreach (string var in myList)
                {
                    mySW.WriteLine(var);
                }
                mySW.Close();
            }


        }

        // LEVEL 1 - ###################################################################################################################################################################
        /// <summary>
        /// Simple Heuristic approach (level 1), to solve the TSP problem
        /// </summary>
        /// <param name="stations">input list of stations</param>
        /// <returns>list of stations that has been sorted using simple heuristic</returns>
        public static List<Station> SimpleHeuristic(List<Station> stations)
        {
            List<Station> stationSimple = new List<Station>();
            stationSimple.Add(stations[0]);
            stationSimple.Add(stations[0]);
            float minLength = 100000;
            float curLength;
            int curMinPos = 0;
            for (int i = 1; i < stations.Count; ++i)
            {
                minLength = 100000;
                curMinPos = 0;// stations.FindIndex(r => r.Name == stations[i].Name);
                for (int j = 0; j < i; ++j)
                {
                    curLength = (float)Math.Round(stations[i].Distance(stationSimple[j]) + stations[i].Distance(stationSimple[j + 1]) - stationSimple[j].Distance(stationSimple[j + 1]), 5);
                    minLength = (float)Math.Round(minLength, 5);
                    if (curLength < minLength)
                    {
                        minLength = curLength;
                        curMinPos = j + 1;
                    }
                }
                stationSimple.Insert(curMinPos, stations[i]);
            }

            return stationSimple;
        }

        //LEVEL 2 - ######################################################################################################################################################
        /// <summary>
        /// Improved Heuristic approach (level 2) to solve the TSP problem
        /// </summary>
        /// <param name="t">input tour</param>
        /// <returns>Tour with shortest distance using improved heuristic</returns>
        public static Tour ImprovedHeuristic(Tour t)
        {
            float minDist = 0;
            float newDist = 0;
            int idx = 1, i = 1, idxNew;
            Tour tNew = new Tour(t);
            // float[] distStation = new float[tNew.station.Count];

            //Station newID;
            //for (int i = 1; i < t.station.Count-1; ++i) 

            while (i < tNew.station.Count - 1)
            {

                idx = i;
                minDist = (float)Math.Round(tNew.CalcDistAround(tNew.station[idx], idx - 1, idx + 1) - tNew.station[idx - 1].Distance(tNew.station[idx + 1]), 5);
                idxNew = idx;
                //  tNew.TourLength(distStation);
                //Console.WriteLine("{0} {1} == {2}  ?", idx, t.station[i].Name, tNew.station[i].Name);
                for (int j = 1; j < tNew.station.Count; ++j)
                {
                    if (idx == j || idx == j - 1)
                    {
                        continue;
                    }
                    else
                    {

                        newDist = (float)Math.Round(tNew.CalcDistAround(tNew.station[idx], j - 1, j) - tNew.station[j - 1].Distance(tNew.station[j]), 5);

                        if (newDist < minDist)
                        {
                            //Console.WriteLine("{0}<{1}?", newDist, minDist);
                            minDist = newDist;
                            idxNew = j;
                        }

                    }

                }
                if (idxNew != idx)
                {
                    tNew.InsertStation(idx, idxNew);

                    i = 1;
                }
                else
                {
                    i++;

                }
            }
            return tNew;
        }

        //LEVEL 3 - ######################################################################################################################################################
        /// <summary>
        /// Exhaustive Search (level 3) approach for TSP problem
        /// </summary>
        /// <param name="tour">input tour</param>
        public static void ExhaustiveSearch(Tour tour)
        {
            List<Station> newStat = new List<Station>();
            for (int i = 1; i < stations.Count; ++i)
            {

                newStat.Add(stations[i]);
            }
            Tour ans = new Tour(newStat, plane);
            newTour = new Tour(ans);
            Generate(ans.station.Count, ans);
        }

        /// <summary>
        /// Generate the permutation of all possible travel path
        /// </summary>
        /// <param name="k">length of tour</param>
        /// <param name="ans">input tour</param>
        public static void Generate(int k, Tour ans)
        {
            if (k == 1)
            {

                float length1 = ans.TourLength() + stations[0].Distance(ans.station[0]) + stations[0].Distance(ans.station[ans.station.Count - 1]);
                float length2 = newTour.TourLength() + stations[0].Distance(newTour.station[0]) + stations[0].Distance(newTour.station[newTour.station.Count - 1]);
                if (length1 < length2)
                {
                    newTour.CopyObj(ans);

                }

            }
            else
            {
                // Generate permutations with kth unaltered
                // Initially k == length(A)
                Generate(k - 1, ans);

                // Generate permutations for kth swapped with each k-1 initial
                for (int i = 0; i < k - 1; i += 1)
                {


                    // Swap choice dependent on parity of k (even or odd)
                    if (k % 2 == 0)
                    {
                        ans.SwapStation(i, k - 1); // zero-indexed, the kth is at k-1
                    }
                    else
                    {
                        ans.SwapStation(0, k - 1);
                    }
                    Generate(k - 1, ans);




                }
            }
        }
        //Level 4 - ##########################################################################################################################################################################################
        /// <summary>
        /// Simulated annealing approach for TSP problem
        /// </summary>
        /// <param name="t">input Tour class</param>
        /// <returns>Sorted tour using simulated annealing</returns>
        public static Tour SimulatedAnnealing(Tour t)
        {
            //primary configuration of cities
            Tour current = new Tour(t);
            //int[] current = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
            //the next configuration of cities to be tested
            //int[] next = new int[current.Length];
            Tour next = new Tour(t);
            int iteration = -1;
            //the probability
            double proba;
            double alpha = 0.999;
            double temperature = 400.0;
            double epsilon = 0.000001;
            double delta;
            double distance = t.TourLength();// TspDataReader.computeDistance(current);
            Console.WriteLine("TL = {0}", distance);

            //while the temperature did not reach epsilon
            while (temperature > epsilon)
            {
                iteration++;

                //get the next random permutation of distances 
                next = ComputeNext(current, next);
                //compute the distance of the new permuted configuration
                delta = next.TourLength() - distance;
                //if the new distance is better accept it and assign it
                //Console.WriteLine("{0}", delta);
                if (delta < 0)
                {
                    current.CopyObj(next);
                    distance = delta + distance;
                    Console.WriteLine(distance);
                }
                else
                {
                    proba = rnd.Next();
                    //if the new distance is worse accept 
                    //it but with a probability level
                    //if the probability is less than 
                    //E to the power -delta/temperature.
                    //otherwise the old value is kept
                    if (proba < Math.Exp(-delta / temperature))
                    {
                        current.CopyObj(next);
                        distance = delta + distance;
                        Console.WriteLine(distance);
                    }
                }
                //cooling process on every iteration
                temperature *= alpha;
                //print every 400 iterations
                // if (iteration % 400 == 0)

            }
            //try
            //{
            //    return "best distance is" + distance;
            //}
            //catch
            //{
            //    return "error";
            //}
            return current;
        }

        /// <summary>
        /// compute a new next configuration
        /// and save the old next as current
        /// </summary>
        /// <param name="c">current configuration</param>
        /// <param name="n">next configuration</param>
        public static Tour ComputeNext(Tour c, Tour n)
        {
            n.CopyObj(c);
            //for (int i = 0; i < c.station.Count; i++)
            //{
            //n.station[i] = c.station[i];
            int i1 = (int)(rnd.Next(c.station.Count - 2) + 1);
            int i2 = (int)(rnd.Next(c.station.Count - 2) + 1);
            //Console.WriteLine("{0} -> {1}", i1, i2);

            n.InsertStation(i1, i2);
            //Station aux = n[i1];
            //n[i1] = n[i2];
            //n[i2] = aux;
            //}
            return n;
        }



    }


}