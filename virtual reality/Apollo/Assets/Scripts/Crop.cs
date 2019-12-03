using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crop : MonoBehaviour
{
	private bool planted;
    // Start is called before the first frame update
    void Start()
    {
		planted = true;
	}

    // Update is called once per frame
    void Update()
    {
        if (planted && transform.parent.tag != "Dirt")
		{
			planted = false;
			GetComponent<Rigidbody>().isKinematic = false;
			GetComponent<Rigidbody>().useGravity = true;
			transform.position = transform.position + new Vector3(0, 0.1f, 0);
		}
    }
}
