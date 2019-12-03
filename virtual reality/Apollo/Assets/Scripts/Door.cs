using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MonoBehaviour
{
	public string name;
	public bool timeout;

	bool open = false;

	float openTime;

	Animation anim;

    // Start is called before the first frame update
    void Start()
    {
		anim = GetComponent<Animation>();
		openTime = 0.0f;
	}

    // Update is called once per frame
    void Update()
    {
        if(open)
		{
			openTime += Time.deltaTime;
		}
		if (timeout && open && openTime > 15.0f)
		{
			Toggle();
		}
    }

	// Toggles the open/closed state of the door
	public void Toggle()
	{
		if (!anim.IsPlaying(name + " Open") && !anim.IsPlaying(name + " Close"))
		{
			if (open)
			{
				anim.Play(name + " Close");
				open = false;
			}
			else
			{
				anim.Play(name + " Open");
				open = true;
				openTime = 0.0f;
			}
		}
	}
}
