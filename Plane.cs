using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigAss
{
    /// <summary>
    /// Class Plane contains plane configuration that had been read from the given plane specs file
    /// The class also convert the received information to TimeSpan type variables
    /// </summary>
    class Plane
    {
        public float range
        {
            get;
            set;
        }
        public float speed
        {
            get;
            set;
        }
        public float takeOff
        {
            get;
            set;

        }
        public float landing
        {
            get;
            set;
        }
        public float fuel
        {
            get;
            set;
        }

        /// <summary>
        /// Constructor of class Plane
        /// </summary>
        /// <param name="range">the range that plane can fly</param>
        /// <param name="speed">speed of plane</param>
        /// <param name="takeOff">take off time</param>
        /// <param name="landing">landing time</param>
        /// <param name="fuel">fueling time</param>
        public Plane(float range, float speed, float takeOff, float landing, float fuel)
        {
            this.range = range;
            this.speed = speed;
            this.takeOff = takeOff;
            this.landing = landing;
            this.fuel = fuel;
        }

        /// <summary>
        /// Convert a given value of time (as a float) to a TimeSpan type variable
        /// </summary>
        /// <param name="value">Time value</param>
        /// <param name="type">Unit of time value, (hour, minute or second)</param>
        /// <returns>TimeSpan variable converted from time value</returns>
        public TimeSpan Convert(float value, string type)
        {
            // Checking if the unit is in hour, minute or second, then convert to TimeSpan variable in the correct format 
            if (type == "hh")
            {
                return TimeSpan.FromHours(value);
            }
            else if (type == "mm")
            {
                return TimeSpan.FromMinutes(value);
            }
            else
            {
                return TimeSpan.Parse(string.Format("0:0:{0}", (value)));
            }
        }
    }
}
