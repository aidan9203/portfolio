using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Button : MonoBehaviour
{
	bool pushed = false;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (pushed)
		{
			transform.localScale = new Vector3(1, 1, 0.5f);
		}
		else
		{
			transform.localScale = new Vector3(1, 1, 1);
		}
    }

	void OnTriggerStay(Collider other)
	{
		pushed = true;
	}

	void OnTriggerExit(Collider other)
	{
		pushed = false;
	}
}
