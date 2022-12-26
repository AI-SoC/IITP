using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class human_action : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Debug.Log("Start");   
    }
    float x, y;
    float delta = - 0.001f;
    float z;
    
    // Update is called once per frame
    void Update()
    {
        int random = Random.Range(1, 5);
        x = transform.position.x;
        y = transform.position.y;
        z = transform.position.z;
        if (random == 1)
            x = x + 0.2f;
        if (random == 2)
            x = x - 0.2f;
        if (random == 3)
            z = z - 0.2f;
        if (random == 4)
            z = z + 0.2f;
        transform.position = new Vector3(x,y,z);

    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.collider.gameObject.CompareTag("Obstacle"))
        {
            Debug.Log("Collision\n");

        } 
        else if (collision.collider.gameObject.CompareTag("Player"))
        {
            Debug.Log("Say sorry\n");
        }
    }
}
