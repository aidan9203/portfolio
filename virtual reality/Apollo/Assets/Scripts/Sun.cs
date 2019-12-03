using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sun : MonoBehaviour
{
	static int dayLength = 1800; //Length of 1 day/night cycle in seconds
	static int distance = 10000; //Sun distance in m

	public double startTime; //Starting time in game hours (0 = midnight)

	double time;	

	// Start is called before the first frame update
	void Start()
    {
		time = startTime * (dayLength / 24);
    }

    // Update is called once per frame
    void Update()
    {
		//Rotate the sun around the origin
		time += Time.deltaTime;
		float angle = (getTimeOfDay() - 6) * 15;
		transform.SetPositionAndRotation(new Vector3(0, distance * Mathf.Sin(Mathf.Deg2Rad * angle), -distance * Mathf.Cos(Mathf.Deg2Rad * angle)), Quaternion.Euler(angle, 0, 0));
    }

	//Time in milliseconds
	public double getTime()
	{
		return time;
	}

	//Time in game days
	public float getTimeDays()
	{
		return (float)(time / dayLength);
	}

	//The game clock time in hours
	public float getTimeOfDay()
	{
		float hours = (float)(time / (dayLength / 24));
		return hours % 24;
	}
}
