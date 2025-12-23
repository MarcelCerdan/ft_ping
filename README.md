# ft_ping

![Score](https://img.shields.io/badge/Score-?%2F100-success?style=for-the-badge&logo=42) ![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge)

**ft_ping** is a C implementation of the standard `ping` command (based on *inetutils*). It sends ICMP ECHO_REQUEST packets to network hosts and listens for ICMP ECHO_REPLY packets to verify connectivity and measure round-trip time.

This project involves deep manipulation of **Raw Sockets**, **ICMP protocol structure**, and **low-level network programming**.

---

## 🚀 Features

- **ICMP Packet Construction**: Manually building packet headers (Type, Code, Checksum, Sequence, ID).
- **Raw Sockets**: Utilizing `SOCK_RAW` to send and receive IP packets directly.
- **Signal Handling**: Catching `SIGINT` (Ctrl+C) to display statistics before exiting.
- **Timing & RTT**: Precise calculation of Round-Trip Time using `gettimeofday`.
- **Error Management**: Handling timeouts, unreachable hosts, and TTL expiration.
- **Statistics**: Calculation of min/avg/max/stddev RTT and packet loss percentage.

### ✨ Bonus Features Implemented
This project includes several advanced options beyond the mandatory requirements:
- **`--ttl <val>`**: Manually set the Time-To-Live (great for simulating network hops).
- **`-W <sec>`**: Set a custom timeout for waiting for a response.
- **`-c <count>`**: Stop after sending a specific number of packets.
- **`-i <sec>`**: Set the interval between packets.
- **`-n`**: Numeric output only (no Reverse DNS lookup).

---

## 🛠️ Installation

```bash
git clone [https://github.com/ton-username/ft_ping.git](https://github.com/ton-username/ft_ping.git)
cd ft_ping
make