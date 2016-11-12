using UnityEngine;
using System.Collections;

public class GrabScript : MonoBehaviour {
    bool attach = false;
    public GameObject parent;
    public GameObject weapon;
	// Use this for initialization
	void Start () {
        attach = false;
	}
	
	// Update is called once per frame
	void Update () {
        Vector3 direction = parent.GetComponent<Rigidbody>().velocity;
        //Debug.Log(direction);
        if (Input.GetAxis("Fire2") == 1)
        {
            Debug.Log("attached");
            attach = true;
        } else
        {
            attach = false;
            if (weapon != null)
            {
                weapon.GetComponent<Rigidbody>().isKinematic = false;
                weapon.transform.parent = null; // disconnect weapon
                weapon.GetComponent<Rigidbody>().AddForce(direction * 200);
                weapon = null; // deset weapon
            }
        }
        //Debug.Log(attach);
	}
    void OnCollisionEnter(Collision col)
    {
        Debug.Log("Yes");
        if (attach)
        {
            //Debug.Log("Trying to grab");
            if (col.gameObject.tag == "Grabbable")
            { // grab object
                //Debug.Log("Object grabbed");
                weapon = col.gameObject;
                weapon.GetComponent<Rigidbody>().isKinematic = true;
                weapon.transform.parent = gameObject.transform; 
            }
        }
    }
}
