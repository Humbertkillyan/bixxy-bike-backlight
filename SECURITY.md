<!-- Security Policy for BIXXY Bike Backlight -->

# Security Policy

## Reporting a Vulnerability

If you discover a security vulnerability in BIXXY, please **do not** open a public issue. Instead, please email us at:

```
security@bixxy-project.dev  (placeholder - replace with actual email)
```

Include:
- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix (if any)

We will:
1. Acknowledge receipt within 24 hours
2. Investigate and assess the severity
3. Develop a fix
4. Release a patched version
5. Credit you (unless you prefer anonymity)

## Supported Versions

| Version | Status | Support Until |
|---------|--------|---------------|
| 1.0.x | Current | 2027-03-29 |
| 0.x | End of Life | - |

## Security Best Practices for Users

When using BIXXY:

1. **Keep Updated** - Always use the latest version
2. **Change Credentials** - Don't use default WiFi passwords
3. **Secure Your Device** - Physical security is also important
4. **Update Firmware** - Check for OTA updates regularly
5. **Monitor Logs** - Review device logs for anomalies

## Security Considerations

### Cloud Communication
- Data is sent over WiFi/MQTT
- Ensure you use secure credentials
- Consider VPN for sensitive deployments

### Hardware
- Keep your Arduino physically secure
- Don't leave sensitive devices unattended
- Use proper enclosures for protection

### Dependencies
- All external libraries are vetted
- Dependencies are locked to specific versions in platformio.ini
- We test all updates before recommending upgrades

## Contact

For security questions or concerns, contact the maintainers directly.

---

**Last Updated**: March 29, 2026
