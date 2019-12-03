using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoverMovement : MonoBehaviour
{
	static int maxForce = 1000;
	static int maxBattery = 1200000;
	float battery;
	public float startBattery; //Starting battery (0-1)

	Rigidbody rb;

	bool driven;

	Transform gagueNeedle;
	GameObject lightRight;
	GameObject lightLeft;

	Transform joystickRight;
	Transform joystickLeft;
	Transform leftTread;
	Transform rightTread;

	float leftThrottle = 0;
	float rightThrottle = 0;

	List<Transform> leftWheels = new List<Transform>();
	List<Transform> rightWheels = new List<Transform>();

	// Start is called before the first frame update
	void Start()
    {
		rb = GetComponent<Rigidbody>();
		driven = false;
		battery = maxBattery * startBattery;

		gagueNeedle = transform.Find("Battery Gague").Find("Battery Gague Needle");
		lightRight = transform.Find("Headlight Right").Find("Headlight Light Right").gameObject;
		lightLeft = transform.Find("Headlight Left").Find("Headlight Light Left").gameObject;

		joystickLeft = transform.Find("Controller Left");
		joystickRight = transform.Find("Controller Right");
		leftTread = transform.Find("Tread Left");
		rightTread = transform.Find("Tread Right");

		leftWheels.Add(transform.Find("Wheel Left 001"));
		leftWheels.Add(transform.Find("Wheel Left 002"));
		leftWheels.Add(transform.Find("Wheel Left 003"));
		leftWheels.Add(transform.Find("Wheel Left 004"));
		leftWheels.Add(transform.Find("Wheel Left 005"));
		leftWheels.Add(transform.Find("Wheel Left 006"));

		rightWheels.Add(transform.Find("Wheel Right 006"));
		rightWheels.Add(transform.Find("Wheel Right 007"));
		rightWheels.Add(transform.Find("Wheel Right 008"));
		rightWheels.Add(transform.Find("Wheel Right 009"));
		rightWheels.Add(transform.Find("Wheel Right 0010"));
		rightWheels.Add(transform.Find("Wheel Right 0011"));
	}

    // Update is called once per frame
    void Update()
    {
		//Battery gague
		float angle = 180 * (battery / maxBattery) - 90;
		gagueNeedle.localEulerAngles = new Vector3(0, 0, angle);

		//Calculate and apply velocities based on throttles
		float rightSpeed = 0;
		float leftSpeed = 0;

		if (Mathf.Abs(leftThrottle) > 0.05f && battery > 0)
		{
			battery -= (Time.deltaTime * Mathf.Abs(leftThrottle)) / 2;
			if (IsTreadGrounded(leftWheels))
			{
				leftSpeed = leftThrottle * 2.5f;
			}
		}
		if (Mathf.Abs(rightThrottle) > 0.05f && battery > 0)
		{
			battery -= (Time.deltaTime * Mathf.Abs(rightThrottle)) / 2;
			battery -= (Time.deltaTime * Mathf.Abs(leftThrottle)) / 2;
			if (IsTreadGrounded(rightWheels))
			{
				rightSpeed = rightThrottle * 2.5f;
			}
		}
		float speed = 0;
		if (Mathf.Abs(rightSpeed) < Mathf.Abs(leftSpeed))
		{
			speed = rightSpeed;
		}
		else
		{
			speed = leftSpeed;
		}

		float rotateSpeed = (rightSpeed - leftSpeed) * 0.3f;

		if (rightSpeed != 0 || leftSpeed != 0)
		{
			rb.velocity = transform.forward * speed;
			rb.angularVelocity = new Vector3(rb.angularVelocity.x, rotateSpeed, rb.angularVelocity.z);
		}
	}

	public void Drive()
	{
		driven = true;
		lightRight.gameObject.SetActive(true);
		lightLeft.gameObject.SetActive(true);

	}

	public void Exit()
	{
		driven = false;
		lightRight.gameObject.SetActive(false);
		lightLeft.gameObject.SetActive(false);
	}

	public void UpdateThrottle(string tag, float pct)
	{
		if (tag == "Rover Controller Left")
		{
			leftThrottle = pct;
		}
		else if (tag == "Rover Controller Right")
		{
			rightThrottle = pct;
		}
		
	}

	bool IsWheelGrounded(Vector3 wheelPosition)
	{
		return Physics.Raycast(wheelPosition, -transform.up, out RaycastHit hit, 0.4f, ~(1 << 9));
	}

	bool IsTreadGrounded(List<Transform> wheels)
	{
		foreach (Transform wheel in wheels)
		{
			if (IsWheelGrounded(wheel.position))
			{
				return true;
			}
		}
		return false;
	}
}
