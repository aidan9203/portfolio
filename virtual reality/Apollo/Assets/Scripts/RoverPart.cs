using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoverPart : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
		foreach(Collider c in GetComponents<Collider>())
		{
			c.isTrigger = true;
		}
		GetComponent<MeshRenderer>().enabled = false;
	}

    // Update is called once per frame
    void Update()
    {
        
    }
	
	//Called when an object enters the collider
	void OnTriggerEnter(Collider other)
	{
		//Adding part to the rover
		if(tag == other.gameObject.tag && transform.parent.gameObject.GetComponent<RoverAssembly>().AddPart(tag))
		{
			foreach (Collider c in GetComponents<Collider>())
			{
				c.isTrigger = false;
			}
			GetComponent<MeshRenderer>().enabled = true;
			Destroy(other.gameObject);
		}
	}
}
