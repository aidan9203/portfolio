using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
	//Useful object components
	CapsuleCollider col;
	Rigidbody rb;
	GameObject cam;

	//In game hands
	public GameObject controllerPrefab;
	GameObject leftController;
	GameObject rightController;

	bool driving;

	// Start is called before the first frame update
	void Start()
    {
		col = GetComponent<CapsuleCollider>();
		rb = GetComponent<Rigidbody>();
		cam = transform.GetChild(0).gameObject;

		FindControllers();

		driving = false;
	}

    // Update is called once per frame
    void Update()
    {
		//Search for controllers if they have not been found yet
		if (!rightController || !leftController)
		{
			FindControllers();
		}

		//Adjust the controller collider to match the camera position and height
		if (driving)
		{
			col.height = cam.transform.localPosition.y - 0.7f;
			Vector3 vehicleVelocity = transform.parent.gameObject.GetComponent<Rigidbody>().velocity;
			rb.velocity = new Vector3(vehicleVelocity.x, rb.velocity.y, vehicleVelocity.z);
		}
		else
		{
			col.height = cam.transform.localPosition.y;
		}
		col.center = new Vector3(cam.transform.localPosition.x, cam.transform.localPosition.y - (col.height / 2), cam.transform.localPosition.z);

		//Driving the rover
		if (Physics.Raycast(new Vector3(cam.transform.position.x, cam.transform.position.y - col.height + 0.1f, cam.transform.position.z), transform.TransformDirection(Vector3.down), out RaycastHit hit, 1.0f, ~(1 << 8)) && hit.collider.tag == "Rover Seat")
		{
			if (!driving && hit.distance < 0.3f)
			{
				driving = true;
				hit.collider.gameObject.transform.parent.gameObject.GetComponent<RoverMovement>().Drive();
				transform.SetParent(hit.collider.gameObject.transform.parent);
			}
		}
		else
		{
			if (driving)
			{
				driving = false;
				transform.parent.gameObject.GetComponent<RoverMovement>().Exit();
				transform.parent = null;
			}
		}
	}
	
	//Finds and assigns controllers
	void FindControllers()
	{
		List<UnityEngine.XR.InputDevice> controllers = new List<UnityEngine.XR.InputDevice>();
		UnityEngine.XR.InputDevices.GetDevices(controllers);
		foreach (UnityEngine.XR.InputDevice controller in controllers)
		{
			//Create right hand object
			if (!leftController && controller.role == UnityEngine.XR.InputDeviceRole.LeftHanded)
			{
				leftController = Object.Instantiate(controllerPrefab, transform.position, Quaternion.identity);
				leftController.transform.SetParent(transform);
				leftController.transform.localScale = new Vector3(-1 * leftController.transform.localScale.x, leftController.transform.localScale.y, leftController.transform.localScale.z);
				leftController.GetComponent<PlayerController>().AssignController(controller, UnityEngine.XR.XRNode.LeftHand);
				leftController.transform.Find("Watch").gameObject.SetActive(true);
			}
			//Create left hand object
			else if (!rightController && controller.role == UnityEngine.XR.InputDeviceRole.RightHanded)
			{
				rightController = Object.Instantiate(controllerPrefab, transform.position, Quaternion.identity);
				rightController.transform.SetParent(transform);
				rightController.GetComponent<PlayerController>().AssignController(controller, UnityEngine.XR.XRNode.RightHand);
			}
		}
	}

	//Jump to the specified location
	public void JumpToPosition(Vector3 pos)
	{
		//Only jump if the player is standing on something
		if (!Physics.Raycast(new Vector3(cam.transform.position.x, cam.transform.position.y - col.height + 0.1f, cam.transform.position.z), transform.TransformDirection(Vector3.down), out RaycastHit hit, 0.3f, ~(1 << 8)))
		{
			return;
		}

		//Calculate the direction and distances to the target
		float direction = Mathf.Atan2(pos.x - cam.transform.position.x, pos.z - cam.transform.position.z);
		float hDistance = Mathf.Sqrt(Mathf.Pow(pos.x - cam.transform.position.x, 2) + Mathf.Pow(pos.z - cam.transform.position.z, 2));
		float vDistance = pos.y - transform.position.y;

		//Calculate the velocities required to get there
		float hVelocity = hDistance / 2.0f;
		float time = hDistance / hVelocity;
		float vVelocity = (0.5f * 1.62f * time) + (2 * (vDistance / time));

		//Apply the velocities (and ensure that the player jumps)
		if (vVelocity < 0.2f) { vVelocity = 0.2f; }
		rb.velocity = new Vector3(hVelocity * Mathf.Sin(direction), vVelocity, hVelocity * Mathf.Cos(direction));
	}

	public GameObject GetLeftController()
	{
		return leftController;
	}

	public GameObject GetRightController()
	{
		return rightController;
	}
}
