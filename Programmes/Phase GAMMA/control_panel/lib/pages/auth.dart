import 'package:control_panel/pages/home.dart';
import 'package:flutter/material.dart';
//import 'package:control_panel/components/custom_button.dart';
import 'package:control_panel/components/custom_textfield.dart';

class LoginPage extends StatelessWidget {
  LoginPage({super.key});

  final usernameController = TextEditingController();
  final passwordController = TextEditingController();

  void onSignIn(BuildContext context) {
    if (usernameController.text == 'admin' &&
        passwordController.text == 'secret') {
      Navigator.pushReplacement(
        context,
        MaterialPageRoute(builder: (context) => const HomePage()),
      );
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
            content: Text('Nom d\'utilisateur ou mot de passe incorrect')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Center(
          child: Column(
            children: [
              const SizedBox(height: 50),
              Image.asset('img/logotype_fond_blanc.png',
                  width: 400, height: 200),
              const Text('Bienvenue sur la plateforme de contrôle de TRAVELERS',
                  style: TextStyle(fontSize: 20)),
              const Divider(),
              const SizedBox(height: 50),
              CustomTextField(
                controller: usernameController,
                labelText: 'Identifiant',
                obscureText: false,
              ),
              const SizedBox(height: 20),
              CustomTextField(
                controller: passwordController,
                labelText: 'Mot de passe',
                obscureText: true,
              ),
              const SizedBox(height: 20),
              const Text('Mot de passe oublié ?'),
              const SizedBox(height: 20),
              CustomButton(
                onTap: () => onSignIn(context),
                text: 'Se connecter',
              ),
            ],
          ),
        ),
      ),
    );
  }
}

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
