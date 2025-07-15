// --- PlayerMovement.cs ---
using UnityEngine;
public class PlayerMovement : MonoBehaviour {
    public float moveSpeed = 5f;
    public Joystick joystick;
    public Rigidbody rb;

    void FixedUpdate() {
        Vector3 direction = new Vector3(joystick.Horizontal, 0, joystick.Vertical);
        rb.MovePosition(transform.position + direction * moveSpeed * Time.fixedDeltaTime);
    }
}

// --- PlayerShooting.cs ---
using UnityEngine;
public class PlayerShooting : MonoBehaviour {
    public GameObject bulletPrefab;
    public Transform firePoint;
    public float fireForce = 700f;

    public void Fire() {
        GameObject bullet = Instantiate(bulletPrefab, firePoint.position, firePoint.rotation);
        Rigidbody rb = bullet.GetComponent<Rigidbody>();
        rb.AddForce(firePoint.forward * fireForce);
        Destroy(bullet, 2f);
    }
}

// --- Bullet.cs ---
using UnityEngine;
public class Bullet : MonoBehaviour {
    public int damage = 20;
    void OnCollisionEnter(Collision collision) {
        EnemyAI enemy = collision.collider.GetComponent<EnemyAI>();
        if (enemy != null) {
            enemy.TakeDamage(damage);
        }
        Destroy(gameObject);
    }
}

// --- EnemyAI.cs ---
using UnityEngine;
using UnityEngine.AI;
public class EnemyAI : MonoBehaviour {
    public NavMeshAgent agent;
    public Transform player;
    public int health = 100;

    void Update() {
        if (player != null) agent.SetDestination(player.position);
    }

    public void TakeDamage(int amount) {
        health -= amount;
        if (health <= 0) Destroy(gameObject);
    }
}

// --- PlayerHealth.cs ---
using UnityEngine;
public class PlayerHealth : MonoBehaviour {
    public int maxHealth = 100;
    private int currentHealth;

    void Start() {
        currentHealth = maxHealth;
    }

    public void TakeDamage(int amount) {
        currentHealth -= amount;
        if (currentHealth <= 0) {
            Debug.Log("Player Died");
            // Reload scene or show game over UI
        }
    }
}

// --- SafeZone.cs ---
using UnityEngine;
public class SafeZone : MonoBehaviour {
    public float shrinkInterval = 10f;
    public float shrinkAmount = 5f;

    void Start() {
        InvokeRepeating("ShrinkZone", shrinkInterval, shrinkInterval);
    }

    void ShrinkZone() {
        transform.localScale -= new Vector3(shrinkAmount, 0, shrinkAmount);
    }

    void OnTriggerExit(Collider other) {
        if (other.CompareTag("Player")) {
            other.GetComponent<PlayerHealth>().TakeDamage(10);
        }
    }
}