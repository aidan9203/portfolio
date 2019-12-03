using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OxygenTank : MonoBehaviour
{
	public float fill; //Initial fill (0-1)

	static int maxFill = 1800;

	float O2;

	Transform gagueNeedle;

	// Start is called before the first frame update
	void Start()
    {
		O2 = maxFill * fill;
		gagueNeedle = transform.Find("Gague").Find("Gague Needle");
	}

    // Update is called once per frame
    void Update()
    {
		float angle = 270 * (O2 / maxFill) - 90;
		gagueNeedle.localEulerAngles = new Vector3(0, 0, angle);
	}

	//Returns the O2 remaining in ms
	public float GetO2()
	{
		return O2;
	}

	//Sets the O2 remaining in ms
	public void SetO2(float val)
	{
		O2 = val;
	}
}
