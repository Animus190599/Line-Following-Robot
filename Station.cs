using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigAss
{
    class Station
    {
        public string Name
        {
            get;
            set;
        }
        public float x
        {
            get;
            set;
        }
        public float y
        {
            get;
            set;
        }

        /// <summary>
        /// Constructor of class Station
        /// </summary>
        /// <param name="Name">Name of station</param>
        /// <param name="x">x position of station</param>
        /// <param name="y">y position of station</param>
        public Station(string Name, float x, float y)
        {
            this.Name = Name;
            this.x = x;
            this.y = y;
        }

        /// <summary>
        /// Construction of class Station
        /// </summary>
        /// <param name="A">Copy the objects in another Station class to this Station class</param>
        public Station(Station A)
        {
            // Assign the objects from station A to this station
            this.Name = A.Name;
            this.x = A.x;
            this.y = A.y;
        }

        /// <summary>
        /// Calculate the distancee between this station to another station
        /// </summary>
        /// <param name="s">another variable with Station class</param>
        /// <returns>The Distance from this station to station s</returns>
        public float Distance(Station s)
        {
            // Calculate the distance base on x,y coordinate of the 2 stations
            return (float)Math.Round(Math.Sqrt(Math.Pow((x - s.x), 2) + Math.Pow((y - s.y), 2)), 5);
        }

        /// <summary>
        /// Compare the objects of a class Station s with this station
        /// </summary>
        /// <param name="s"></param>
        /// <returns>return a logical variable stating whether or not the 2 classes are identical</returns>
        public bool Equals(Station s)
        {
            // Compare each objects from each station together
            if (this.Name == s.Name && this.x == s.x && this.y == s.y)
            {
                return true;
            }
            return false;
        }
    }
}
