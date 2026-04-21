"""Модуль кликер-игры «Бой с монстром» с графическим интерфейсом Tkinter.

Игрок атакует монстра, зарабатывает монеты, покупает зелья для восстановления здоровья.
С каждым новым уровнем монстр становится сильнее.
"""

import tkinter as tk
from tkinter import messagebox
import random
from typing import Optional


class ClickerGame:
    """Основной класс игры, управляющий логикой и интерфейсом."""

    def __init__(self, root: tk.Tk) -> None:
        """Инициализация игры и создание виджетов.

        Args:
            root: корневое окно Tkinter.
        """
        self.root: tk.Tk = root
        self.root.title("Бой с монстром")
        self.root.geometry("500x400")

        # Игровые переменные
        self.coins: int = 0
        self.click_power: int = 1

        # Параметры монстра
        self.monster_max_health: int = 100
        self.monster_health: int = self.monster_max_health
        self.monster_level: int = 1

        # Параметры игрока
        self.player_max_health: int = 100
        self.player_health: int = self.player_max_health

        # Стоимость и эффект зелья
        self.potion_cost: int = 20
        self.potion_heal: int = 30

        self.create_widgets()

    def create_widgets(self) -> None:
        """Создание и размещение всех элементов интерфейса."""
        # Монетки
        self.coins_label = tk.Label(
            self.root,
            text=f"Монетки: {self.coins}",
            font=("Arial", 20),
            fg="gold"
        )
        self.coins_label.pack(pady=10)

        # Здоровье игрока
        self.player_health_frame = tk.Frame(self.root)
        self.player_health_frame.pack(pady=5)

        self.player_health_label = tk.Label(
            self.player_health_frame,
            text=f"Здоровье игрока: {self.player_health}/{self.player_max_health}",
            font=("Arial", 14),
            fg="red"
        )
        self.player_health_label.pack(side=tk.LEFT, padx=10)

        # Здоровье монстра
        self.monster_health_frame = tk.Frame(self.root)
        self.monster_health_frame.pack(pady=5)

        self.monster_health_label = tk.Label(
            self.monster_health_frame,
            text=f"Здоровье монстра: {self.monster_health}/{self.monster_max_health}",
            font=("Arial", 14),
            fg="green"
        )
        self.monster_health_label.pack(side=tk.LEFT, padx=10)

        self.monster_level_label = tk.Label(
            self.monster_health_frame,
            text=f"Уровень: {self.monster_level}",
            font=("Arial", 12)
        )
        self.monster_level_label.pack(side=tk.LEFT, padx=10)

        # Кнопка атаки
        self.attack_button = tk.Button(
            self.root,
            text="АТАКОВАТЬ МОНСТРА!",
            font=("Arial", 18),
            bg="#FF5722",
            fg="white",
            width=20,
            height=2,
            command=self.on_attack
        )
        self.attack_button.pack(pady=20)

        # Кнопка покупки зелья
        self.potion_button = tk.Button(
            self.root,
            text=f"Купить зелье (+{self.potion_heal} HP)",
            font=("Arial", 14),
            bg="#4CAF50",
            fg="white",
            command=self.buy_potion
        )
        self.potion_button.pack(pady=10)

        # Стоимость зелья
        self.potion_cost_label = tk.Label(
            self.root,
            text=f"Стоимость зелья: {self.potion_cost} монет",
            font=("Arial", 12)
        )
        self.potion_cost_label.pack()

        # Сообщение о действии
        self.message_label = tk.Label(
            self.root,
            text="",
            font=("Arial", 12),
            fg="blue"
        )
        self.message_label.pack(pady=10)

    def on_attack(self) -> None:
        """Обработчик нажатия кнопки атаки.

        Игрок наносит урон монстру, получает монеты и принимает ответный удар.
        Если здоровье монстра или игрока достигает нуля, запускаются
        соответствующие события (появление нового монстра или сброс игры).
        """
        if self.player_health <= 0:
            messagebox.showinfo("Игра окончена", "Вы проиграли! Монстр победил вас.")
            self.reset_game()
            return

        if self.monster_health <= 0:
            self.spawn_new_monster()
            return

        # Урон по монстру
        base_damage: int = random.randint(1, 5)
        level_bonus: int = 2 * self.monster_level
        damage_to_monster: int = base_damage + level_bonus
        self.monster_health -= damage_to_monster

        # Монеты за удар
        coins_earned: int = random.randint(1, 5) + self.monster_level
        self.coins += coins_earned

        # Ответный удар монстра
        monster_damage: int = random.randint(1, 5 + self.monster_level * 2)
        self.player_health -= monster_damage

        # Обновление сообщения
        message: str = (f"Вы нанесли {damage_to_monster} урона! "
                        f"Получено {coins_earned} монет. "
                        f"Монстр нанес {monster_damage} урона.")
        self.message_label.config(text=message)

        # Проверка смерти монстра
        if self.monster_health <= 0:
            self.monster_health = 0
            message = f"Монстр повержен! Вы получили бонус {coins_earned * 2} монет!"
            self.message_label.config(text=message)
            self.coins += coins_earned  # бонус за победу
            self.root.after(1000, self.spawn_new_monster)

        # Проверка смерти игрока
        if self.player_health <= 0:
            self.player_health = 0
            messagebox.showinfo("Игра окончена", "Вы проиграли! Монстр победил вас.")
            self.reset_game()

        self.display()

    def spawn_new_monster(self) -> None:
        """Создание нового монстра следующего уровня."""
        self.monster_level += 1
        self.monster_max_health = 100 + (self.monster_level - 1) * 50
        self.monster_health = self.monster_max_health
        self.click_power += 1  # Увеличение силы атаки с уровнем
        self.message_label.config(text=f"Новый монстр {self.monster_level} уровня появился!")
        self.display()

    def buy_potion(self) -> None:
        """Покупка и использование зелья восстановления здоровья."""
        if self.coins >= self.potion_cost:
            if self.player_health >= self.player_max_health:
                messagebox.showinfo("Полное здоровье", "У вас уже полное здоровье!")
                return

            self.coins -= self.potion_cost
            old_health: int = self.player_health
            self.player_health = min(self.player_max_health, self.player_health + self.potion_heal)
            healed: int = self.player_health - old_health
            self.message_label.config(text=f"Вы выпили зелье и восстановили {healed} здоровья!")
            self.display()
        else:
            messagebox.showinfo("Недостаточно монет",
                                f"Нужно {self.potion_cost} монет для зелья!")

    def reset_game(self) -> None:
        """Сброс всех параметров игры до начальных."""
        self.coins = 0
        self.click_power = 1
        self.monster_level = 1
        self.monster_max_health = 100
        self.monster_health = self.monster_max_health
        self.player_max_health = 100
        self.player_health = self.player_max_health
        self.message_label.config(text="")
        self.display()

    def display(self) -> None:
        """Обновление всех текстовых меток интерфейса."""
        self.coins_label.config(text=f"Монетки: {self.coins}")
        self.player_health_label.config(
            text=f"Здоровье игрока: {self.player_health}/{self.player_max_health}")
        self.monster_health_label.config(
            text=f"Здоровье монстра: {self.monster_health}/{self.monster_max_health}")
        self.monster_level_label.config(text=f"Уровень: {self.monster_level}")
        self.potion_cost_label.config(text=f"Стоимость зелья: {self.potion_cost} монет")


if __name__ == "__main__":
    root = tk.Tk()
    game = ClickerGame(root)
    root.mainloop()

