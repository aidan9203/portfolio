using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoverAssembly : MonoBehaviour
{
	int wheels = 0;
	int frame = 1;
	int motors = 0;
	int controllers = 0;
	bool battery = false;
	bool seat = false;
	bool storage = false;

	bool complete = false;
	bool treads = false;
	bool wires = false;
	bool joysticks = false;
	bool chains = false;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (!complete && storage && seat && battery && controllers == 2 && motors == 2 && frame == 4 && wheels == 22)
		{
			complete = true;
		}
		if (!treads && wheels == 22)
		{
			transform.Find("Tread Left").gameObject.SetActive(true);
			transform.Find("Tread Right").gameObject.SetActive(true);
		}
		if (!chains && motors == 2 && wheels == 22)
		{
			transform.Find("Motor Chain Left").gameObject.SetActive(true);
			transform.Find("Motor Chain Right").gameObject.SetActive(true);
		}
		if (!wires && battery && motors == 2)
		{
			transform.Find("Wires Negative").gameObject.SetActive(true);
			transform.Find("Wires Positive").gameObject.SetActive(true);
		}
		if (!joysticks && battery && controllers == 2)
		{
			transform.Find("Wires Front").gameObject.SetActive(true);
			transform.Find("Controller Left").gameObject.SetActive(true);
			transform.Find("Controller Right").gameObject.SetActive(true);
			transform.Find("Battery Gague").gameObject.SetActive(true);
			transform.Find("Headlight Right").gameObject.SetActive(true);
			transform.Find("Headlight Left").gameObject.SetActive(true);
		}
	}

	//Informs the rover that a part has been added
	public bool AddPart(string t)
	{
		if (t == "Rover Frame Side")
		{
			frame++;
			return true;
		}
		else if (t == "Rover Frame Upper" && battery && motors == 2)
		{
			frame++;
			return true;
		}
		else if (t == "Rover Motor")
		{
			motors++;
			return true;
		}
		else if (t == "Rover Battery")
		{
			battery = true;
			return true;
		}
		else if (t == "Rover Wheel" && frame >= 3)
		{
			wheels++;
			return true;
		}
		else if (t == "Rover Controller" && frame == 4)
		{
			controllers++;
			return true;
		}
		else if (t == "Rover Seat" && frame == 4)
		{
			seat = true;
			return true;
		}
		else if (t == "Rover Storage" && frame == 4)
		{
			storage = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	//Returns whether the rover is fully assembled
	public bool IsComplete()
	{
		return complete;
	}
}
