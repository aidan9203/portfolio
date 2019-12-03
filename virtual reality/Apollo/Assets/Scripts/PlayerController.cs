using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
	//The teleport marker object
	public GameObject markerPrefab;
	GameObject marker;

	//Unity XR information about this controller
	UnityEngine.XR.InputDevice controller;
	UnityEngine.XR.XRNode hand;

	//The object currently being held
	GameObject held;
	GameObject joystick;

	GameObject player;

	Animation anim;

	//Last position for calculating velocities
	Vector3 lastPosition;

	bool climbing;

	//Called when object is created
	void Start()
    {
		held = null;
		joystick = null;
		lastPosition = transform.position;
		climbing = false;

		player = transform.parent.gameObject;

		//Ignore collisions with the player object
		Physics.IgnoreLayerCollision(8, 8);

		anim = transform.GetChild(0).gameObject.GetComponent<Animation>();
		anim["Grip"].speed = 0;
	}

	//Called once per frame
	void Update()
    {
		//Update controller object position
		transform.localPosition = UnityEngine.XR.InputTracking.GetLocalPosition(hand);
		transform.rotation = UnityEngine.XR.InputTracking.GetLocalRotation(hand);

		//Teleportation
		controller.TryGetFeatureValue(UnityEngine.XR.CommonUsages.primaryButton, out bool teleport);
		if (teleport && Arccast(transform.position, transform.rotation * Vector3.forward, out RaycastHit hit, 3.0f, 1.62f, 0.5f, ~(1 << 8)))
		{
			//Create or update the marker object when the button is being pressed
			if (marker)
			{
				marker.transform.position = hit.point;
			}
			else
			{
				marker = Object.Instantiate(markerPrefab, hit.point, Quaternion.identity);
			}
		}
		//When the button is released destroy the marker and jump to its location
		if (!teleport && marker)
		{
			transform.parent.GetComponent<PlayerMovement>().JumpToPosition(marker.transform.position);
			Object.Destroy(marker);
		}

		//Letting go of objects
		controller.TryGetFeatureValue(UnityEngine.XR.CommonUsages.trigger, out float hold);
		if (hold <= 0.2)
		{
			if (climbing)
			{
				climbing = false;
				player.GetComponent<Rigidbody>().useGravity = true;
				player.GetComponent<Rigidbody>().velocity = -0.5f * (transform.position - lastPosition) / Time.deltaTime;
			}
			if (held != null)
			{
				Drop();
			}
		}

		//Climbing
		if (climbing)
		{
			player.transform.position -= (transform.position - lastPosition);
			player.GetComponent<Rigidbody>().velocity = new Vector3(0, 0, 0);
			player.GetComponent<Rigidbody>().useGravity = false;
		}

		//Rover joystick control
		if (joystick != null && hold > 0.2)
		{
			Vector3 localControllerPosition = joystick.transform.parent.InverseTransformPoint(transform.position) - joystick.transform.localPosition;
			if (joystick.tag == "Lander Joystick")
			{
				float angleX = -(Mathf.Rad2Deg * Mathf.Atan2(localControllerPosition.y, localControllerPosition.z));
				float angleY = (Mathf.Rad2Deg * Mathf.Atan2(localControllerPosition.x, localControllerPosition.z));
				if (angleX > 30) { angleX = 30; }
				else if (angleX < -30) { angleX = -30; }
				if (angleY > 30) { angleY = 30; }
				else if (angleY < -30) { angleY = -30; }
				joystick.transform.localEulerAngles = new Vector3(angleX, angleY, 0);
			}
			else if (joystick.tag == "Lander Throttle")
			{
				float posY = (joystick.transform.parent.InverseTransformPoint(transform.position)).y + 2;
				if (posY > 4) { posY = 4; }
				else if (posY < -4) { posY = -4; }
				joystick.transform.localPosition = new Vector3(0, posY, 1.18f);
			}
			else
			{
				float angle = -(Mathf.Rad2Deg * Mathf.Atan2(localControllerPosition.y, localControllerPosition.z + 1.0f)) + 90;
				if (angle > 30) { angle = 30; }
				else if (angle < -30) { angle = -30; }
				joystick.transform.localEulerAngles = new Vector3(angle - 90, 0, 0);
				joystick.transform.parent.gameObject.GetComponent<RoverMovement>().UpdateThrottle(joystick.tag, angle / 30.0f);
			}
		}
		else if (joystick != null && hold <= 0.2)
		{
			if (joystick.tag == "Lander Joystick")
			{
				joystick.transform.localEulerAngles = new Vector3(0, 0, 0);
			}
			else if (joystick.tag == "Lander Throttle") { }
			else
			{
				joystick.transform.localEulerAngles = new Vector3(-90, 0, 0);
				joystick.transform.parent.gameObject.GetComponent<RoverMovement>().UpdateThrottle(joystick.tag, 0);
			}
			joystick = null;
		}

		//Set the animation frame
		anim["Grip"].time = hold;
		anim.Play("Grip");

		//Stop the held object if it is moving
		if (held)
		{
			held.GetComponent<Rigidbody>().velocity = Vector3.zero;
			held.GetComponent<Rigidbody>().angularVelocity = Vector3.zero;
		}

		//Update the previous position
		lastPosition = transform.position;
	}

	//Called every frame there is another collider in the trigger
	void OnTriggerStay(Collider other)
	{
		controller.TryGetFeatureValue(UnityEngine.XR.CommonUsages.trigger, out float hold);
		if (held == null && joystick == null && hold > 0.2)
		{
			if (other.gameObject.layer == 10)
			{
				PickUp(other.gameObject);
			}
			else if (other.tag == "Rover Controller Left" || other.tag == "Rover Controller Right")
			{
				joystick = other.gameObject;
			}
			else if (other.tag == "Lander Joystick" || other.tag == "Lander Throttle")
			{
				joystick = other.gameObject;
			}
			else if (other.tag == "Handle")
			{
				other.transform.parent.gameObject.GetComponent<Door>().Toggle();
			}
			else if (other.tag == "Lid")
			{
				other.gameObject.GetComponent<Door>().Toggle();
			}
			else if (other.tag == "Climbable")
			{
				climbing = true;
			}
		}
	}

	//Called when a collider exits the trigger
	void OnTriggerExit(Collider other)
	{
		//Drop the object if it moves out of the hand
		if (held != null && other.gameObject == held)
		{
			Drop();
		}
	}

	//Assign the XR controller and node to this object
	public void AssignController(UnityEngine.XR.InputDevice c, UnityEngine.XR.XRNode n)
	{
		controller = c;
		hand = n;
	}

	//Raycast with gravity simulaton
	bool Arccast(Vector3 position, Vector3 direction, out RaycastHit hit, float speed, float gravity, float interval, int mask)
	{
		//For each segment calculate the start and end points using the current velocity, gravity, and segment interval (simulated travel time for each segment)
		Vector3 velocity = direction * (speed * interval);
		Vector3 start = position;
		Vector3 end = start + velocity;
		while (!Physics.Linecast(start, end, out hit, mask))
		{
			if (end.y < -500) { return false; }
			velocity = new Vector3(velocity.x, velocity.y - (gravity * interval), velocity.z);
			start = end;
			end += velocity;
		}
		return true;
	}

	//Picking up objects
	void PickUp(GameObject obj)
	{
		//If the object is held by the other controller drop it so it can be picked up by this one
		if (obj.transform.parent != null && obj.transform.parent.gameObject.tag == "Hand")
		{
			obj.transform.parent.gameObject.GetComponent<PlayerController>().Drop();		
		}

		held = obj;
		held.transform.SetParent(transform);
		held.GetComponent<Rigidbody>().useGravity = false;
	}

	//Dropping objects
	public void Drop()
	{
		if (held == null) { return; }

		held.transform.SetParent(null);
		held.GetComponent<Rigidbody>().useGravity = true;
		held.GetComponent<Rigidbody>().velocity = (transform.position - lastPosition) / Time.deltaTime;
		held = null;
	}

	public GameObject GetHeld()
	{
		return held;
	}
}