import 'package:flutter/material.dart';

class CustomButton extends StatelessWidget {
  final VoidCallback onTap;
  final String text;

  const CustomButton({super.key, required this.onTap, required this.text});

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 500),
      child: ElevatedButton(
        onPressed: onTap,
        style: ElevatedButton.styleFrom(
          backgroundColor: const Color(0xFF1331F5),
          padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 20),
          textStyle: const TextStyle(
            fontSize: 20,
            fontWeight: FontWeight.bold,
          ),
        ),
        child: Text(text),
      ),
    );
  }
}

class ButtonSwitch extends StatelessWidget {
  final VoidCallback onTap;
  final String text;
  final bool isOn;

  const ButtonSwitch(
      {super.key, required this.onTap, required this.text, required this.isOn});

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 50,
      width: 280,
      child: ElevatedButton(
        onPressed: onTap,
        style: ElevatedButton.styleFrom(
          backgroundColor: const Color(0xFF1331F5),
          padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 20),
          textStyle: const TextStyle(
            fontSize: 20,
            fontWeight: FontWeight.bold,
          ),
        ),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text(text),
            Container(
              width: 20,
              height: 20,
              decoration: BoxDecoration(
                shape: BoxShape.circle,
                color: isOn ? Colors.green : Colors.red,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
