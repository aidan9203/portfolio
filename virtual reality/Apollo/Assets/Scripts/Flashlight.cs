using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Flashlight : MonoBehaviour
{
	GameObject spotLight;

    // Start is called before the first frame update
    void Start()
    {
		spotLight = transform.Find("Spot Light").gameObject;
	}

    // Update is called once per frame
    void Update()
    {
		//Only turn the flashlight on if it is being held
        if (transform.parent != null && transform.parent.tag == "Hand")
		{
			spotLight.SetActive(true);
		}
		else
		{
			spotLight.SetActive(false);
		}
    }
}
