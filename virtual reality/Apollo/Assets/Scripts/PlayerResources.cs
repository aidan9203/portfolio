using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerResources : MonoBehaviour
{
	static int maxOxygen = 900;
	static int maxFood = 1800;
	static int maxWater = 900;

	public float startOxygen; //(0-1)
	public float startFood; //(0-1)
	public float startWater; //(0-1)

	float oxygen;
	float food;
	float water;

	GameObject cam;

    // Start is called before the first frame update
    void Start()
    {
		oxygen = maxOxygen * startOxygen;
		food = maxFood * startFood;
		water = maxWater * startWater;

		cam = transform.GetChild(0).gameObject;
	}

    // Update is called once per frame
    void Update()
    {
		oxygen -= Time.deltaTime;
		food -= Time.deltaTime;
		water -= Time.deltaTime;

		GameObject leftController = GetComponent<PlayerMovement>().GetLeftController();
		GameObject rightController = GetComponent<PlayerMovement>().GetRightController();

		GameObject leftHeld = null;
		GameObject rightHeld = null;

		if (leftController != null)
		{
			leftHeld = leftController.GetComponent<PlayerController>().GetHeld();
			Transform watch = leftController.transform.Find("Watch");
			watch.Find("Oxygen Needle").localEulerAngles = new Vector3(0, 0, (-180 * (oxygen / maxOxygen) + 90));
			watch.Find("Food Needle").localEulerAngles = new Vector3(0, 0, (90 * (food / maxFood)) - 90);
			watch.Find("Water Needle").localEulerAngles = new Vector3(0, 0, (-90 * (water / maxWater) + 90));

		}
		if (rightController != null)
		{
			rightHeld = rightController.GetComponent<PlayerController>().GetHeld();
		}

		if (leftHeld != null)
		{
			float distance = Vector3.Distance(cam.transform.position, leftHeld.transform.position);
			if (distance < 0.5f)
			{
				if (Eat(leftHeld))
				{
					leftController.GetComponent<PlayerController>().Drop();
					Destroy(leftHeld);
				}
			}
		}
		if (rightHeld != null)
		{
			float distance = Vector3.Distance(cam.transform.position, rightHeld.transform.position);
			if (distance < 0.5f)
			{
				if (Eat(rightHeld))
				{
					rightController.GetComponent<PlayerController>().Drop();
					Destroy(rightHeld);
				}
			}
		}
	}

	bool Eat(GameObject obj)
	{
		if (obj.tag == "Apple" || obj.tag == "Orange" || obj.tag == "Potato")
		{
			if (maxFood - food > 450)
			{
				food += 450;
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (obj.tag == "Bananna" || obj.tag == "Carrot")
		{
			if (maxFood - food > 300)
			{
				food += 300;
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (obj.tag == "Sandwich")
		{
			if (maxFood - food > 900)
			{
				food += 900;
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (obj.tag == "Water" && obj.transform.Find("Water").gameObject.activeSelf)
		{
			if (maxWater - water > 450)
			{
				water += 450;
				obj.transform.Find("Water").gameObject.SetActive(false);
			}
			return false;
		}
		else if (obj.tag == "Oxygen")
		{
			float o2 = obj.GetComponent<OxygenTank>().GetO2();
			if ((maxOxygen - oxygen) < o2)
			{
				obj.GetComponent<OxygenTank>().SetO2(o2 - (maxOxygen - oxygen));
				oxygen = maxOxygen;
			}
			else
			{
				oxygen += o2;
				obj.GetComponent<OxygenTank>().SetO2(0);
			}
			return false;
		}
		else
		{
			return false;
		}
	}
}