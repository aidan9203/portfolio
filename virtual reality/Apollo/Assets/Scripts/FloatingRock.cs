using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FloatingRock : MonoBehaviour
{
	public float scale;
	public float speed;

	float startY;
	float offset;

    // Start is called before the first frame update
    void Start()
    {
		startY = transform.position.y;
		offset = 3.14f * Random.value;
    }

    // Update is called once per frame
    void Update()
    {
		float y = startY + scale * Mathf.Cos((speed * Time.time) + offset);
		transform.position = new Vector3(transform.position.x, y, transform.position.z);
    }
}
