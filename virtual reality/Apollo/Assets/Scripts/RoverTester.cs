using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoverTester : MonoBehaviour
{
	bool completed = false;

	GameObject r;

    // Start is called before the first frame update
    void Start()
    {
		r = transform.Find("Rover").gameObject;
		r.GetComponent<RoverAssembly>().AddPart("Rover Frame Side");
		r.GetComponent<RoverAssembly>().AddPart("Rover Frame Side");
		r.GetComponent<RoverAssembly>().AddPart("Rover Battery");
		r.GetComponent<RoverAssembly>().AddPart("Rover Motor");
		r.GetComponent<RoverAssembly>().AddPart("Rover Motor");
		r.GetComponent<RoverAssembly>().AddPart("Rover Frame Upper");
		r.GetComponent<RoverAssembly>().AddPart("Rover Seat");
		r.GetComponent<RoverAssembly>().AddPart("Rover Storage");
		r.GetComponent<RoverAssembly>().AddPart("Rover Controller");
		r.GetComponent<RoverAssembly>().AddPart("Rover Controller");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
		r.GetComponent<RoverAssembly>().AddPart("Rover Wheel");
	}

    // Update is called once per frame
    void Update()
    {
		//Rover assembly
		if (!completed && r.GetComponent<RoverAssembly>().IsComplete())
		{
			foreach (Transform part in transform.Find("Rover"))
			{
				part.gameObject.GetComponent<MeshRenderer>().enabled = true;
				foreach (Collider c in part.gameObject.GetComponents<Collider>())
				{
					if (c.tag != "Rover Controller Left" && c.tag != "Rover Controller Right")
						c.isTrigger = false;
				}
			}
			completed = true;
		}
	}
}
