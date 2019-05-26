using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigAss
{
    /// <summary>
    ///  Contains a list of station, and plane specs
    ///  This class contains methods to calculate distance, time as well as swaping stations around
    /// </summary>
    class Tour
    {
        public List<Station> station
        {
            get;
            set;
        }
        public Plane plane
        {
            get;
            set;
        }

        /// <summary>
        /// Constructor of class Tour
        /// </summary>
        /// <param name="station">List of Station class</param>
        /// <param name="plane">plane specs read from file</param>
        public Tour(List<Station> station, Plane plane)
        {
            this.station = station;
            this.plane = plane;
        }

        /// <summary>
        /// Constructor of class Tour using another class Tour as parameter
        /// </summary>
        /// <param name="A">Variable with same class</param>
        public Tour(Tour A)
        {
            // passing each object in class Tour A to initialize this class Tour
            this.station = new List<Station>();
            for (int i = 0; i < A.station.Count; ++i)
            {
                this.station.Add(A.station[i]);
            }

            this.plane = A.plane;
        }

        /// <summary>
        /// Coby objects from another class to this class
        /// </summary>
        /// <param name="A"></param>
        public void CopyObj(Tour A)
        {
            // Copy the value of each object in Tour A to this Tour class
            this.plane = A.plane;
            for (int i = 0; i < A.station.Count; ++i)
            {
                //this.station[i].Name = A.station[i].Name;
                //this.station[i].x = A.station[i].x;
                //this.station[i].y = A.station[i].y;
                this.station[i] = A.station[i];
            }
        }

        /// <summary>
        /// Calculate the total distance travel through every station of this Tour
        /// </summary>
        /// <returns>The total distance that plane will travel in this class</returns>
        public float TourLength()
        {
            // total distance travelled will be stored here
            float total = 0;
            int i = 0;
            // iterate through each station in this class, and calculate the distance from
            // this station to the next one.
            for (i = 0; i < this.station.Count - 1; ++i)
            {
                total += (float)station[i].Distance(station[i + 1]);

            }
            return total;
        }


        //public void TourLength(float[] arr)
        //{
        //    int i = 0;
        //    //arr = new float[this.station.Count];
        //    for (i = 0; i < this.station.Count - 1; ++i)
        //    {
        //        arr[i] = (float)station[i].Distance(station[i + 1]);
        //    }
        //}

        /// <summary>
        /// Calculate Distance from this station s to the 2 stations surround it in the list,
        /// in respect to the order of the stations
        /// </summary>
        /// <param name="s">station in the middle</param>
        /// <param name="prev">index of station before station s in the list</param>
        /// <param name="next">index of the next station after station s in the list</param>
        /// <returns>Distance from this station to the neareast 2 tations around</returns>
        public float CalcDistAround(Station s, int prev, int next)
        {
            return (float)Math.Round(s.Distance(this.station[prev]) + s.Distance(this.station[next]), 5);
        }

        /// <summary>
        /// Insert station from this index to the new index
        /// </summary>
        /// <param name="oldID">The index of the station that needed to be moved</param>
        /// <param name="newID">The new index where the station should be in</param>
        public void InsertStation(int oldID, int newID)
        {
            // Store the value of the station before deleting it
            Station curStation = new Station(this.station[oldID]);
            // Delete the station at index oldID
            station.RemoveAt(oldID);
            //Insert station at the newID
            //Since when we delete the oldID, the whole list will move to the left, 
            //so if newID>oldID, the newID will be smaller (1 index)
            if (newID > oldID)
            {
                station.Insert(newID - 1, curStation);
            }
            else
            {
                station.Insert(newID, curStation);
            }
        }

        /// <summary>
        /// Swap 2 stations from at oldID and newID
        /// </summary>
        /// <param name="oldID">index of the first station</param>
        /// <param name="newID">index of second station</param>
        public void SwapStation(int oldID, int newID)
        {
            // Provide a temporary variable to store the first station
            Station temp = this.station[oldID];
            //Assign the first station with second station value
            this.station[oldID] = this.station[newID];
            //Assign the second station with first station value
            this.station[newID] = temp;
        }

        /// <summary>
        /// Calculate the time travelled from station of a given index to the next one in string format 
        /// (the string format can be later on converted to TimeSpan variable)
        /// </summary>
        /// <param name="i">index of the current station</param>
        /// <returns>Time travelled in string format</returns>
        public string PathTime(int i)
        {
            //Calculate the time to travel from station[i] to station[i+1]
            float addTime = (float)(Math.Round(this.station[i].Distance(this.station[i + 1]) / plane.speed * 60.0 + plane.takeOff + plane.landing, MidpointRounding.AwayFromZero));
            int hour = 0;
            //Convert the calculated time to hours and minutes
            if (addTime >= 60)
            {
                hour = (int)(addTime / 60);
                addTime %= 60;
            }
            //Format the hours and minutes into a string that's convenient to convert to TimeSpan variable
            if (hour > 24)
            {
                return string.Format("{0}:{1}:{2}:0", (int)(hour / 24), hour - 24, addTime);
            }
            else
            {
                return string.Format("{0}:{1}:0", hour, addTime);
            }
        }

    }
}
