using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Switch : MonoBehaviour
{
	bool flipped = false;

	// Start is called before the first frame update
	void Start()
	{

	}

	// Update is called once per frame
	void Update()
	{
		//Switch flipping
		if (flipped)
		{
			transform.GetChild(0).localEulerAngles = new Vector3(12, 0, 0);
		}
		else
		{
			transform.GetChild(0).localEulerAngles = new Vector3(-12, 0, 0);
		}
	}

	void OnTriggerEnter(Collider other)
	{
		flipped = !flipped;
	}
}
